#include "../include/ModelFileWidget.h"

#include "../include/ColorWidget.h"
#include "../include/ProcessPlanDialog.h"

#include <Wt/WText.h>
#include <Wt/WImage.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WDialog.h>
#include <Wt/WMessageBox.h>
#include <Wt/WPushButton.h>

#include "../../libfxtract/include/sheet-metal-component/bend/ModelBend.h"
#include "../../libfxtract/include/sheet-metal-component/SheetMetal.h"

#include "../../dbdao/include/ModelFileDao.h"
#include "../../dbdao/include/BendFeatureDao.h"
#include "../../dbdao/include/ToolDao.h"
#include "../../dbdao/include/ProcessPlanDao.h"
#include "../../dbdao/include/BendSequenceDao.h"
#include "../../dbdao/include/MaterialDao.h"

#include "../../libseqgen/include/BendSequenceGenerator.h"

#include <vector>
#include <map>
#include <utility>
#include <future>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/assume_abstract.hpp>

using namespace Fxt;
namespace fs = std::filesystem;

namespace {
    const Standard_Real kMachineModuleLength = 1000.0;
    double total_time = 0;
    std::map<std::string, std::vector<ModelBend>> modelFeatureMap;
    std::unique_ptr<Fxt::Model> test;

    std::string save(const Fxt::Model &m){
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << m;

        return ss.str();
    }

    std::unique_ptr<Model> restore(std::string restoreStr)
    {
        auto restored = std::make_unique<Model>();

        std::stringstream iss(restoreStr);
        boost::archive::text_iarchive oa(iss);
        oa >> *restored;

        return restored;
    }

    static Model extractFeatures(const char *modelFile)
    {
        test = std::make_unique<Model>(modelFile);
        test->init();
        test->classifyFaces();
        test->computeBendAngles();

        if(test->cleanModel())
            std::cout << "++++++++++++++++CLEANED!!++++++++++++++++" << std::endl;
        
        test->printBendInfo();

        std::cout << "extractFeatures------" << std::endl;
        return *test;
    }

    static long double procModelFile(const char *modelFile)
    {
        std::future<Model> futureData = std::async(std::launch::async, extractFeatures, modelFile);

        auto modelFeatures = futureData.get();

        modelFeatureMap[std::string(modelFile)] = modelFeatures.getBends();

        std::cout << "procModelFile---------" << std::endl;

        return modelFeatures.getThickness();
    }

    static size_t computerModules(std::vector<ModelBend> bendFeatureData)
    {
        Standard_Real longest = 0.0;
        for (auto bend : bendFeatureData) {
            if (longest < roundd(bend.getBendLength())) {
                longest = roundd(bend.getBendLength());
            }
        }

        int num_of_modules = 1;

        if (longest > kMachineModuleLength){
            num_of_modules = longest / kMachineModuleLength;

            if ((static_cast<int>(longest) % static_cast<int>(kMachineModuleLength)) > 0)
                ++num_of_modules;
        }

        return num_of_modules;
    }

}

class ColorWidget : public Wt::WContainerWidget
{
public:
    ColorWidget() : Wt::WContainerWidget()
    {
        setMaximumSize(10, 10);
        setPadding(10);
        addStyleClass("model-color-red", true);
    }

    void setIconColor(ProcessLevel pL) 
    {
        switch (pL)
        {
        case ProcessLevel::FEATURE_EXTRACTED:
            addStyleClass("model-color-orange", true);
            break;
        case ProcessLevel::PROCESS_PLAN_GEN:
            addStyleClass("model-color-green", true);
            break;
        default:
            break;
        }
    }
};

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

ModelFileWidget::~ModelFileWidget(){}

void ModelFileWidget::processModelFile()
{
    std::vector<ModelBend> bendFeatureData;
    std::string filename = modelFile_->getCadFileDir().toUTF8() + modelFile_->getCadFileName().toUTF8(); 

    switch (modelFile_->getProcessLevel())
    {
    case ProcessLevel::UNPROCESSED:
        std::cout << "Feature extraction started..." << std::endl;

        extractFeatures(modelFile_, session_, filename);

                for (const dbo::ptr<Tool> tool : tools){
                    toolName = tool->tool_id;
                    std::cout << "FOUND TOOL : " << toolName << std::endl;
                    break;
                }

                dbo::ptr<Material> material = session_.find<Material>()
                                    .where("material_name = ?").bind(modelFile_->modelMaterial);     

                std::cout << "Material : " << material->material_name
                          << "K Factor : " << material->k_factor
                          << "TStrength : " << material->tensile_strength << std::endl;
                          
                auto bend_force = a.getBendLength() * std::pow(thickness, 2) 
                                * material->k_factor 
                                * material->tensile_strength
                                / (8 * thickness);

                dbo::ptr<BendFeature> bendFeature(Wt::cpp14::make_unique<BendFeature>());
                BendFeature *b = bendFeature.modify();
                
                session_.add(bendFeature);

                b->bend_id = a.getFaceId();
                b->face_id1 = a.getJoiningFaceID1();
                b->face_id2 = a.getJoiningFaceID2();
                b->bend_angle = a.getBendAngle();
                b->bend_radius = a.getBendRadius();
                b->bend_length = a.getBendLength();
                b->modelFile = modelFile_;
                b->bend_direction = 0;
                b->bending_tool_id = toolName;
                b->bend_force = bend_force;

                modelFile_.modify()->thickness = thickness;
                modelFile_.modify()->bendingForce = bend_force;

                session_.flush();

                t.commit();
            }

            dbo::Transaction t(session_);
            
            modelFile_.modify()->processLevel = ProcessLevel::FEATURE_EXTRACTED;
            modelFile_.modify()->nbends = bendFeatureData.size();
            modelFile_.modify()->nModules = computerModules(bendFeatureData);
            modelFile_.modify()->modelData = save(*test);
            modelFile_.modify()->feature_recognition_time = total_time;

            icons_->setIconColor(ProcessLevel::FEATURE_EXTRACTED);

            t.commit();
        }

        // std::cout << "\n\n\n" << std::endl;

        session_.modelFeaturesExtracted().emit(modelFile_);
        std::cout << "Done......................." << std::endl;
        break;
    case ProcessLevel::FEATURE_EXTRACTED:
        total_time = 0;
        std::cout << "Process Planning started..." << std::endl;        
        {
            std::string restoreStr;
            std::vector<int> bids; 
            
            dbo::Transaction t(session_);

            restoreStr = modelFile_.modify()->getModelData().toUTF8();

            auto& bendFeatures = modelFile_.modify()->bendFeatures;
            
            auto restored = restore(restoreStr);

            std::vector<size_t> bDirs;
            for(auto& bend : bendFeatures) {
                restored->getBendMap().find(bend->bend_id)->second.setBendDirection(bend->bend_direction);
            }

            modelFile_.modify()->modelData = save(*restored);
            restoreStr = modelFile_.modify()->modelData.toUTF8();
            restored = restore(restoreStr);

            for (const auto& bend : restored->getBendMap()) {
                bids.push_back(static_cast<int>(bend.first));
            }

            std::cout << ">>>>>>>>>> Filename : " << restored->getModelFile() << std::endl;

            int startTime = clock();
            Sequence bestSequence = generateBendingSeq(*restored, bids);
            int stopTime = clock();

            total_time = (stopTime - startTime) / double(CLOCKS_PER_SEC);
            // std::cout << ">>>>>>>>>> Bend sequence time : " << total_time <<std::endl;
            // std::cout << total_time << std::endl;

        generateBendingSequence(modelFile_, session_);

            session_.add(processPlan);

            pp->dateCreated = Wt::WDateTime::currentDateTime();
            pp->no_flips = bestSequence.nFlips;
            pp->no_rotations = bestSequence.nRotations;
            pp->process_planning_time += total_time + modelFile_.modify()->feature_recognition_time;
            pp->modelFile = modelFile_;
            pp->quantity = 1;
            pp->tool_distance = bestSequence.distance;

            for(const auto& bs : bestSequence.chromosome )
            {
                dbo::ptr<BendSequence> bendSeq = session_.add(Wt::cpp14::make_unique<BendSequence>());
              
                bendSeq.modify()->bend_id = bs;
                bendSeq.modify()->processPlan = processPlan;
            }

            dbo::ptr<MachineParam> machineParam(Wt::cpp14::make_unique<MachineParam>());
            MachineParam *mp = machineParam.modify();

            session_.add(machineParam);
            mp->num_tools = bestSequence.nTools;
            mp->processPlan = processPlan;

            modelFile_.modify()->processLevel = ProcessLevel::PROCESS_PLAN_GEN;

            session_.flush();            

            t.commit(); 
        }

        std::cout << "\n" << std::endl;
        
        icons_->setIconColor(ProcessLevel::PROCESS_PLAN_GEN );
        // session_.modelFeaturesExtracted().emit(modelFile_);
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
    }
}

void ModelFileWidget::rm()
{
    dbo::Transaction t(session_);


void ModelFileWidget::extractFeatures(Wt::Dbo::ptr<ModelFile>& modelFile, Session& session, std::string filename)
{
    if (modelFeatureCache.find(filename) != modelFeatureCache.end())
    {
        auto bendFeatureData = modelFeatureCache[filename];
    } else {
        int startTime = clock();

        auto sheetMetalFeatureModel = std::make_shared<Fxt::SheetMetalComponent::SheetMetal>();
        auto loggingService = std::make_shared<Fxt::Logging::StandardOutputLogger>(session_.user()->name);
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

            auto tool = toolDao.get(bend->getBendFeature()->getBendAngle());

            float bendingForce = materialDao.getBendingForce(
                                    modelFile_->getModelMaterial().toUTF8(),
                                    sheetMetalFeatureModel->getThickness(), 
                                    bend->getBendFeature()->getBendLength());

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

void ModelFileWidget::generateBendingSequence(Wt::Dbo::ptr<ModelFile>& modelFile_, Session& session_)
{
    double total_time = 0.0;
    BendFeatureDao bendFeatureDao { session_ };
    ModelFileDao modelFileDao { session_ };
    ProcessPlanDao processPlanDao { session_ };
    BendSequenceDao bendSequenceDao { session_ };

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