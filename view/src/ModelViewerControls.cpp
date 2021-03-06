#include "../include/ModelViewerControls.h"

#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WProgressBar.h>
#include <Wt/WTimer.h>
#include <Wt/WMessageBox.h>

#include "../include/FileDropApplication.h"
#include "../include/ProcessPlanDialog.h"
#include "../include/FeatureDialog.h"

#include "../../dbdao/include/ProjectDao.h"
#include "../../dbdao/include/ModelFileDao.h"
#include "../../dbdao/include/FolderStructure.h"

#include "../../libfxtract/include/Computation.h"

namespace {

    long long dupCount = 0;
    std::string userProjectFolder;
    std::string userTempFolder;

    auto processString = [](double x) {
        std::ostringstream os;
        os << x;
        return os.str();
    };
}

using namespace Fxt;

ModelViewerControls::ModelViewerControls(Session& session, const std::string uploadPath, dbo::ptr<Project> project)
    :WContainerWidget(),
    session_(session),
    project_(project),
    uploadPath_(uploadPath)
{
    userProjectFolder = uploadPath_ + "/" + session_.user()->name.toUTF8() + "/" + project_->titleToUrl() + "/";
    userTempFolder = uploadPath_ + "/" + session_.user()->name.toUTF8() + "/temp/"; 

	auto controls_ = Wt::cpp14::make_unique<Wt::WTemplate>(tr("tpl.template"));
    controls_->addStyleClass("controls", true);    
    
    auto addModelButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	addModelButton_ = controls_->bindWidget("add-model", std::move(addModelButton));
    addModelButton_->addStyleClass("btn btn-floating btn-success btn-circle btn-md");
    addModelButton_->setIcon("icons/add_model.svg");
    addModelButton_->setToolTip("Upload CAD file(s)");
    addModelButton_->clicked().connect([=] {
        addModelDialog();
    });

    // auto bendingSeqButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	// bendingSeqButton_ = controls_->bindWidget("bs-gen", std::move(bendingSeqButton));
    // bendingSeqButton_->addStyleClass("btn btn-floating btn-success btn-circle btn-md");
    // bendingSeqButton_->setIcon("icons/settings.svg");
    // bendingSeqButton_->setToolTip("Generate the Bending Sequence");
    // bendingSeqButton_->clicked().connect([=] {
    //     bendingSeqDialog();
    // });

    auto featureButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	featureButton_ = controls_->bindWidget("fag", std::move(featureButton));
    featureButton_->addStyleClass("btn btn-success btn-circle btn-md");
    featureButton_->setIcon("icons/fag.svg");
    featureButton_->setToolTip("Current CAD model features");
    featureButton_->clicked().connect([=] {
        featureDialog();
    });

    auto SettingButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	SettingButton_ = controls_->bindWidget("settings", std::move(SettingButton));
    SettingButton_->addStyleClass("btn btn-success btn-circle btn-md");
    SettingButton_->setIcon("icons/bsg.svg");
    SettingButton_->setToolTip("Display processing plan");
    SettingButton_->clicked().connect([=] {
        settingsDialog();
    }); 

    addWidget(std::move(controls_));

    session_.modelFileClicked().connect(this, &ModelViewerControls::updateProjectModel);
}

ModelViewerControls::~ModelViewerControls(){}

void ModelViewerControls::updateProjectModel(dbo::ptr<ModelFile> modelFile)
{
    modelFile_ = modelFile;
}

void ModelViewerControls::saveNewModel(std::vector<std::string> files, std::string materialType)
{   
    for (std::vector<std::string>::iterator iter = begin(files); (iter != end(files)); ++iter) {

        std::string modelDir, modelFileName, modelObjFile;
        
        std::string filename = iter->substr(0, iter->rfind("."));
        std::string ext = iter->substr(iter->find(".") + 1);

        Wt::Http::ParameterValues::iterator obj;

        if (ext == "obj") {
            // If the first uploaded file is an .obj file then search for its step/iges file
            obj = std::find(files.begin(), files.end(), filename + ".stp");

            if (obj == end(files))
            {
                obj = std::find(files.begin(), files.end(), filename + ".step");
            } 

            if (obj == end(files))
            {
                obj = std::find(files.begin(), files.end(), filename + ".STEP");
            }                           

            // obj is the model file name if it is found
            modelFileName = obj != end(files) ? *obj : std::string();

            // use obj (if found) as the folder name for the uploaded model and its .obj file
            modelDir = userProjectFolder + (obj != end(files) ? *obj : std::string());
            modelObjFile = *iter;  // current iter is the .obj file uploaded
        } else {
            // If first uploaded file is a step/iges file then find its .obj file
            obj = std::find(files.begin(), files.end(), filename + ".obj");

            // Current iter is the folder name for the uploaded model and its .obj file
            modelDir = userProjectFolder + *iter;

            // modelObjFile = obj if .obj file for the step/iges has been found
            modelObjFile = obj != end(files) ? *obj : std::string();

            // modelFileName = iter if .obj has been found
            modelFileName = obj != end(files) ? *iter  : std::string();
        }

        // discard the uploaded files if their corresponding model or .obj files have not been found
        if (modelFileName.empty() || modelObjFile.empty()) {
            deleteFolder(userTempFolder);
            createFolders(userTempFolder);
            
            std::cerr << "Error : No correcsponding step/iges/obj file uploaded!!" << std::endl;
            break;
        }

        if (fileExists(modelDir)) {
            ++dupCount;
            modelDir += "_" + std::to_string(dupCount) + "/";
        } else  {
            modelDir +=  "/";
        }

        createFolders(modelDir);

        if (obj != files.end()) {
            std::filesystem::path pobj { userTempFolder + *obj }; 
            std::filesystem::path p { userTempFolder + *iter };

            renameFile( p, modelDir + *iter);                
            renameFile( pobj, modelDir + *obj);

            files.erase(obj);            
        } else {
            renameFile( { userTempFolder + *iter }, modelDir + *iter);
        }

        using namespace Fxt::Dao;

        ModelFileDao modelFileDao { session_ };
        dbo::ptr<ModelFile> modelFile = modelFileDao.insert(materialType, modelFileName, modelObjFile, modelDir, project_);
        
        session_.modelFileChanged().emit(modelFile);
    
        deleteFolder({ userTempFolder + *iter });
    }
}

void ModelViewerControls::addModelDialog()
{
    auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("Upload an Iges/Step File"));

    Wt::WLabel *label1 = dialog->contents()->addNew<Wt::WLabel>("Model Material");
    Wt::WComboBox *materialType = dialog->contents()->addNew<Wt::WComboBox>();
    label1->setBuddy(materialType);
    materialType->setMargin(10, Wt::Side::Bottom);
    materialType->setPlaceholderText("Select model material");
    materialType->addItem("Aluminium");
    materialType->addItem("Mild Steel");
    materialType->addItem("Stainless Steel");

    dialog->contents()->addStyleClass("form-group");

    Fxt::FileDropApplication *dropWidget = dialog->contents()->addNew<Fxt::FileDropApplication>(session_, uploadPath_);
    dropWidget->setWidth(570);
    dropWidget->addStyleClass("centered", true);
    dialog->contents()->addStyleClass("form-group");

    Wt::WPushButton *ok =
        dialog->footer()->addNew<Wt::WPushButton>("OK");
    ok->setDefault(true);

    Wt::WPushButton *cancel =
        dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog->rejectWhenEscapePressed();

    /*
     * Accept the dialog
     */
    ok->clicked().connect([=] {
            dialog->accept();
    });

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect([=] {
      if (dialog->result() == Wt::DialogCode::Accepted) {
        saveNewModel(dropWidget->uploadedFiles(), materialType->currentText().toUTF8());
      }
        removeChild(dialog);
    });

    dialog->show();
}

void ModelViewerControls::featureDialog()
{
    if (!modelFile_)
    {
        Wt::WMessageBox::show(
                        "FxTract", "<b>Select a model file for which to view bend features from the \"Project Model Files\" panel.</b>",
                        Wt::StandardButton::Ok
                    );
        return;
    }

    auto dialog = addChild(Wt::cpp14::make_unique<FeatureDialog>(session_, "Model File : " + modelFile_->getCadFileName().toUTF8(), modelFile_));
    dialog->setWidth(900);
    dialog->show();
}

void ModelViewerControls::settingsDialog()
{
    if (!modelFile_)
    {
        Wt::WMessageBox::show(
                        "FxTract", "<b>Select a model file for which to view bend features from the \"Project Model Files\" panel.</b>",
                        Wt::StandardButton::Ok
                    );
        return;
    }

    if (modelFile_->getProcessLevel() != ProcessLevel::PROCESS_PLAN_GEN)
    {
        Wt::WMessageBox::show(
            "FxTract", "<b> generate the model process plan.</b>", 
            Wt::StandardButton::Ok
        );

        return;
    }
    
    auto d = addChild(Wt::cpp14::make_unique<ProcessPlanDialog>(session_, "Process Plan", modelFile_));
    d->show();
}

void ModelViewerControls::bendingSeqDialog()
{
    if (!modelFile_)
    {
        Wt::WMessageBox::show(
                        "FxTract", "<b>Select a model file for which to generate a bend sequence from the \"Project Model Files\" panel.</b>",
                        Wt::StandardButton::Ok
                    );
        return;
    }
    auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("Generating bending sequence..."));
    
    Wt::WProgressBar *bar = dialog->contents()->addNew<Wt::WProgressBar>();
    bar->setWidth(570);
    bar->setRange(0, 10);

    auto intervalTimer = addChild(Wt::cpp14::make_unique<Wt::WTimer>());
    intervalTimer->setInterval(std::chrono::milliseconds(1000));
    intervalTimer->start();

    // do some stuff

    intervalTimer->timeout().connect([=] {
        bar->setValue(bar->value() + 1);
        if (bar->value() == 10) {
            intervalTimer->stop();
            removeChild(dialog);
        }
    });

    dialog->show();
}