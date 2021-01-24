#include "../include/ModelFileWidget.h"

#include "../include/ProcessPlanDialog.h"

#include <Wt/WText.h>
#include <Wt/WImage.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WMessageBox.h>

#include "../../libfxtract/include/sheet-metal-component/BoostSerializer.h"

#include "../../libfxtract/include/cad-file-reader/CadFileReader.h"
#include "../../libfxtract/include/cad-file-reader/CadFileReaderFactory.h"

#include "../../logging/include/LoggingFacility.h"
#include "../../logging/include/StandardOutputLogger.h"

#include "../../dbdao/include/ModelFileDao.h"
#include "../../dbdao/include/BendFeatureDao.h"
#include "../../dbdao/include/ToolDao.h"
#include "../../dbdao/include/ProcessPlanDao.h"
#include "../../dbdao/include/BendSequenceDao.h"
#include "../../dbdao/include/MaterialDao.h"
#include "../../dbdao/include/FolderStructure.h"

#include "../../libseqgen/include/BendSequenceGenerator.h"

using namespace Fxt;
using namespace Fxt::Dao;

ModelFileWidget::ModelFileWidget(Session& session, dbo::ptr<ModelFile> model)
    : WContainerWidget(),
    session_(session),
    modelFile_(model)
{
    addStyleClass("model-box", true);
    setPadding(0);

    hBox_ = setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    icons_ = hBox_->addWidget(Wt::cpp14::make_unique<ColorWidget>());

    dbo::Transaction t(session_);

    icons_->setIconColor(modelFile_->getProcessLevel());

    t.commit();

    auto text = hBox_->addWidget(Wt::cpp14::make_unique<Wt::WText>(modelFile_->getCadFileName()));
    text->clicked().connect([=]{
        session_.modelFileClicked().emit(modelFile_);
        processModelFile();
    });

    deleteIcon_ = hBox_->addWidget(Wt::cpp14::make_unique<Wt::WImage>("icons/clear.svg"));
    deleteIcon_->setMaximumSize(20, 20);
    deleteIcon_->clicked().connect([=] {
        deleteModelFile();
    });
}

void ModelFileWidget::processModelFile()
{
    switch (modelFile_->getProcessLevel())
    {
    case ProcessLevel::UNPROCESSED:
        std::cout << "Feature extraction started..." << std::endl;

        extractFeatures();

        icons_->setIconColor(ProcessLevel::FEATURE_EXTRACTED);

        session_.modelFeaturesExtracted().emit(modelFile_);
        break;
    case ProcessLevel::FEATURE_EXTRACTED:
        std::cout << "Process Planning started..." << std::endl;        

        generateBendingSequence();

        icons_->setIconColor(ProcessLevel::PROCESS_PLAN_GEN );
        break;
    case ProcessLevel::PROCESS_PLAN_GEN:

        {
            std::cout << "Processing already complete!" << std::endl;
            auto d = addChild(Wt::cpp14::make_unique<ProcessPlanDialog>(session_, "Process Plan", modelFile_));
            d->show();
        }
        
        break;
    
    default:
        break;
    }
}

void ModelFileWidget::deleteModelFile()
{
    Wt::StandardButton answer = Wt::WMessageBox::show(
                                    "FxTract", "<p>Delete \"" + modelFile_->getCadFileName() + "\" ?</p>",
                                    Wt::StandardButton::Ok | Wt::StandardButton::Cancel
                                );
    if (answer == Wt::StandardButton::Ok) {
        rm();
        session_.modelFileDeleted().emit(modelFile_);
    }
}

void ModelFileWidget::rm()
{
    deleteFolder(modelFile_->getCadFileDir().toUTF8());

    ModelFileDao modelFileDao { session_ };
    modelFileDao.deleteProject(modelFile_);
    
    this->removeFromParent();
}

void ModelFileWidget::extractFeatures()
{
    std::string filename = modelFile_->getCadFileDir().toUTF8() + modelFile_->getCadFileName().toUTF8();

    if (modelFeatureCache.find(filename) != modelFeatureCache.end())
    {
        auto bendFeatureData = modelFeatureCache[filename];
    } else {
        int startTime = clock();

        auto sheetMetalFeatureModel = std::make_shared<Fxt::SheetMetalComponent::SheetMetal>();
        auto loggingService = std::make_shared<Fxt::Logging::StandardOutputLogger>(session_.user()->name.toUTF8());
        auto cadReaderFactory = std::make_shared<Fxt::CadFileReader::CadFileReaderFactory>(loggingService);

        auto cadFileReader = cadReaderFactory->createReader(filename.c_str());

        cadFileReader->extractFaces(sheetMetalFeatureModel, filename.c_str());

        modelFeatureCache[filename] = sheetMetalFeatureModel->getBends();

        int stopTime = clock();

        auto total_time = (stopTime - startTime) / double(CLOCKS_PER_SEC);

        ToolDao toolDao { session_ };
        MaterialDao materialDao { session_ };
        ModelFileDao modelFileDao { session_ };
        BendFeatureDao bendFeatureDao { session_ };

        std::vector<float> bendingForces;
        for (auto& [bendId, bend]: modelFeatureCache[filename]) {
            loggingService->writeInfoEntry("Computations on the extracted features...."
                                    , std::to_string(bend->getBendFeature()->getBendAngle()) + "degrees");

            auto tool = toolDao.get(bend->getBendFeature()->getBendAngle());

            loggingService->writeInfoEntry("Tool assignment....");

            float bendingForce = materialDao.getBendingForce(
                                    modelFile_->getModelMaterial().toUTF8(),
                                    sheetMetalFeatureModel->getThickness(), 
                                    bend->getBendFeature()->getBendLength());

            loggingService->writeInfoEntry("Computations of the bending force....");

            bendingForces.push_back(bendingForce);

            bendFeatureDao.insert(*bend, modelFile_, sheetMetalFeatureModel->getThickness());
        }

        float bendingForce = *(std::max_element(bendingForces.begin(), bendingForces.end()));

        modelFileDao.update(modelFile_, total_time);
        modelFileDao.update(modelFile_, bendingForce);
        modelFileDao.update(modelFile_, ProcessLevel::FEATURE_EXTRACTED);
        modelFileDao.update(modelFile_, sheetMetalFeatureModel->getThickness());
        modelFileDao.update(modelFile_, save(sheetMetalFeatureModel));
    }
}

void ModelFileWidget::generateBendingSequence()
{
    double total_time = 0.0;
    BendFeatureDao bendFeatureDao { session_ };
    ModelFileDao modelFileDao { session_ };

    auto bendFeatures = bendFeatureDao.get(modelFile_);            
    auto serializedObjectData = modelFileDao.getStringifiedModelData(modelFile_);
    auto unStringifiedSheetMetalObj = restore(serializedObjectData);

    int value = 0;
    std::vector<int> vec(unStringifiedSheetMetalObj->getBends().size());
    std::generate(begin(vec), end(vec), [&value]{ value += 1; return value; });

    int startTime = clock();

    auto bendSequenceGenerator = std::make_shared<BendSequenceGenerator>(vec, unStringifiedSheetMetalObj);
    bendSequenceGenerator->generateBendingSequence();

    int stopTime = clock();
    total_time = (stopTime - startTime) / double(CLOCKS_PER_SEC);

    ProcessPlanDao processPlanDao { session_ };
    auto processPlan = processPlanDao.insert(*bendSequenceGenerator, modelFile_, total_time);

    BendSequenceDao bendSequenceDao { session_ };
    bendSequenceDao.insert(bendSequenceGenerator->getSequence(), processPlan);

    modelFileDao.update(modelFile_, ProcessLevel::PROCESS_PLAN_GEN);
}