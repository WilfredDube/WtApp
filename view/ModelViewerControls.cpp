#include "ModelViewerControls.h"

#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTextArea.h>
#include <Wt/WFileDropWidget.h>
#include <Wt/WComboBox.h>
#include <Wt/WProgressBar.h>
#include <Wt/WTimer.h>
#include <Wt/WTable.h>
#include <Wt/WMessageBox.h>
#include <Wt/WInPlaceEdit.h>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cmath>

#include "FileDropApplication.h"
#include "../model/Project.h"
#include "../model/ModelFile.h"
#include "../libfxtract/include/ModelMath.h"

namespace fs = std::filesystem;

namespace {
    #include "../Utils/Utils.h"

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
    addModelButton_->setToolTip("Add model file(s)");
    addModelButton_->clicked().connect([=] {
        addModelDialog();
    });

    auto bendingSeqButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	bendingSeqButton_ = controls_->bindWidget("bs-gen", std::move(bendingSeqButton));
    bendingSeqButton_->addStyleClass("btn btn-floating btn-success btn-circle btn-md");
    bendingSeqButton_->setIcon("icons/bsg.svg");
    bendingSeqButton_->setToolTip("Generate the Bending Sequence");
    bendingSeqButton_->clicked().connect([=] {
        bendingSeqDialog();
    });

    auto featureButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	featureButton_ = controls_->bindWidget("fag", std::move(featureButton));
    featureButton_->addStyleClass("btn btn-success btn-circle btn-md");
    featureButton_->setIcon("icons/fag.svg");
    featureButton_->setToolTip("View Model Face Adjacency Graph");
    featureButton_->clicked().connect([=] {
        featureDialog();
    });

    auto SettingButton = Wt::cpp14::make_unique<Wt::WPushButton>();
	SettingButton_ = controls_->bindWidget("settings", std::move(SettingButton));
    SettingButton_->addStyleClass("btn btn-success btn-circle btn-md");
    SettingButton_->setIcon("icons/settings.svg");
    SettingButton_->setToolTip("Change Display settings");
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
        fs::path p { userTempFolder + *iter };
        
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
            fs::remove_all(userTempFolder);
            fs::create_directory(userTempFolder);
            
            std::cerr << "Error : No correcsponding step/iges/obj file uploaded!!" << std::endl;
            break;
        }

        if (fs::exists(modelDir)) {
            ++dupCount;
            modelDir += "_" + std::to_string(dupCount) + "/";
        } else  {
            modelDir +=  "/";
        }

        fs::create_directory(modelDir);

        if (obj != files.end()) {
            fs::path pobj { userTempFolder + *obj };        
            fs::rename( p, modelDir + *iter);                
            fs::rename( pobj, modelDir + *obj);
            files.erase(obj);            
        } else {
            fs::rename( p, modelDir + *iter);
        }
        
        dbo::Transaction t(session_);

        dbo::ptr<ModelFile> modelFile(Wt::cpp14::make_unique<ModelFile>());
        ModelFile *m = modelFile.modify();
        
        session_.add(modelFile);

        m->tensileStrength = tensileStrength(materialType);
        m->modelMaterial = materialType;
        m->modelFile = modelFileName;
        m->modelObjFile = modelObjFile;
        m->modelFileDir = modelDir;
        m->processLevel = ProcessLevel::UNPROCESSED;
        m->author = session_.user();
        m->project = project_;
        m->uploadDate = Wt::WDateTime::currentDateTime();
        session_.modelFileChanged().emit(modelFile);            

        session_.flush();

        t.commit();            
    
        fs::remove_all(p);
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

    auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("Model File : " + modelFile_->modelFile.toUTF8()));
    dialog->setWidth(900);

    Wt::WPushButton *cancel =
        dialog->footer()->addNew<Wt::WPushButton>("OK");
    dialog->rejectWhenEscapePressed();

    auto bendFeaturesTable = dialog->contents()->addNew<Wt::WTable>();
    bendFeaturesTable->setHeaderCount(1);
    bendFeaturesTable->setWidth(Wt::WLength("100%"));
    bendFeaturesTable->elementAt(0, 0)->addNew<Wt::WText>("Bend ID");
    bendFeaturesTable->elementAt(0, 1)->addNew<Wt::WText>("Face ID");
    bendFeaturesTable->elementAt(0, 2)->addNew<Wt::WText>("Face ID");
    bendFeaturesTable->elementAt(0, 3)->addNew<Wt::WText>("Bend Angle");
    bendFeaturesTable->elementAt(0, 4)->addNew<Wt::WText>("Bend Length");
    bendFeaturesTable->elementAt(0, 5)->addNew<Wt::WText>("Bend Radius");
    bendFeaturesTable->elementAt(0, 6)->addNew<Wt::WText>("Bend Directions");
    bendFeaturesTable->elementAt(0, 7)->addNew<Wt::WText>("Tool Name");
    bendFeaturesTable->setStyleClass("myTable");//table-bordered table-hover table-striped");

    dbo::Transaction t(session_);

    BendFeatures bf = modelFile_->bendFeatures;

    int rowCount = bendFeaturesTable->rowCount();

    for(auto& bend : bf) {
        bendFeaturesTable->elementAt(rowCount, 0)->addNew<Wt::WText>("B" + std::to_string(bend->bend_id));
        bendFeaturesTable->elementAt(rowCount, 1)->addNew<Wt::WText>("F" + std::to_string(bend->face_id1));
        bendFeaturesTable->elementAt(rowCount, 2)->addNew<Wt::WText>("F" + std::to_string(bend->face_id2));
        bendFeaturesTable->elementAt(rowCount, 3)->addNew<Wt::WText>(processString(bend->bend_angle));
        bendFeaturesTable->elementAt(rowCount, 4)->addNew<Wt::WText>(processString(bend->bend_length));
        bendFeaturesTable->elementAt(rowCount, 5)->addNew<Wt::WText>(processString(bend->bend_radius));
        auto comboBox = bendFeaturesTable->elementAt(rowCount, 6)->addNew<Wt::WComboBox>();
        comboBox->addItem("Select the bend direction");
        comboBox->addItem("Inside");
        comboBox->addItem("Outside");

        // if(bend->bend_direction == 1 || bend->bend_direction == 2)
        // {
            comboBox->setCurrentIndex(roundd(bend->bend_direction));
        // }    

        bendFeaturesTable->elementAt(rowCount, 7)->addNew<Wt::WText>(bend->bending_tool_id.c_str());

        comboBox->changed().connect([=]{
            Wt::WString bendDirStr = comboBox->currentText();
            size_t bendDir;

            if (bendDirStr == "Inside")
            {
                bendDir = 1;
            } else {
                bendDir = 2;
            }

            if (bendDir == 1 || bendDir == 2) {
                dbo::Transaction t(session_);

                bend.modify()->bend_direction = bendDir;

                t.commit();
            }            
        });

        ++rowCount;
    }

    t.commit();

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

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

    // auto t = Wt::cpp14::make_unique<Wt::WTemplate>(Wt::WString::tr("pp.template"));

    auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("MODEL PROCESS PLAN"));
    // dialog->setWidth(Wt::WLength::Auto);

    Wt::WPushButton *cancel =
        dialog->footer()->addNew<Wt::WPushButton>("OK");
    dialog->rejectWhenEscapePressed();

    // auto bendFeaturesTable = (Wt::cpp14::make_unique<Wt::WTable>());
    auto t = dialog->contents()->addWidget(Wt::cpp14::make_unique<Wt::WTemplate>(Wt::WString::tr("pp.template")));
    dialog->setWidth(1200);
    
    auto plannerName = t->bindWidget("part_name", Wt::cpp14::make_unique<Wt::WText>());
    plannerName->setText(modelFile_->modelFile);

    auto part_no = t->bindWidget("part_no", Wt::cpp14::make_unique<Wt::WInPlaceEdit>("Enter part number"));
    part_no->setPlaceholderText("Enter part number");

    plannerName = t->bindWidget("process_code", Wt::cpp14::make_unique<Wt::WText>());
    plannerName->setText(modelFile_->project->title);

    plannerName = t->bindWidget("planner_name", Wt::cpp14::make_unique<Wt::WText>());
    plannerName->setText(modelFile_->author->name);

    plannerName = t->bindWidget("module_no", Wt::cpp14::make_unique<Wt::WText>());
    plannerName->setText(std::to_string(modelFile_->nModules));

    plannerName = t->bindWidget("material_name", Wt::cpp14::make_unique<Wt::WText>());
    plannerName->setText(modelFile_->modelMaterial);

    auto ipe = t->bindWidget("moderator", Wt::cpp14::make_unique<Wt::WInPlaceEdit>("Click to enter moderator"));
    ipe->setPlaceholderText("Enter moderator's name");

    auto bendFeaturesTable  = t->bindWidget("feature_table", Wt::cpp14::make_unique<Wt::WTable>());

    bendFeaturesTable->setWidth(Wt::WLength("100%"));

    bendFeaturesTable->elementAt(0, 0)->addNew<Wt::WText>("<b>OPERATION NO</b>");
    bendFeaturesTable->elementAt(0, 1)->addNew<Wt::WText>("<b>BEND ID</b>");
    bendFeaturesTable->elementAt(0, 2)->addNew<Wt::WText>("<b>BEND ANGLE</b>");
    bendFeaturesTable->elementAt(0, 3)->addNew<Wt::WText>("<b>BEND RADIUS</b>");
    bendFeaturesTable->elementAt(0, 4)->addNew<Wt::WText>("<b>BEND DIRECTION</b>");
    bendFeaturesTable->elementAt(0, 5)->addNew<Wt::WText>("<b>BEND LENGTH</b>");
    bendFeaturesTable->elementAt(0, 6)->addNew<Wt::WText>("<b>TOOLS</b>");

    dbo::Transaction transaction(session_);

    BendFeatures bf = modelFile_->bendFeatures;

    int rowCount = bendFeaturesTable->rowCount();

    int size = 1;
    for(auto& bend : bf) {
        bendFeaturesTable->elementAt(rowCount, 0)->addNew<Wt::WText>(std::to_string(size));
        bendFeaturesTable->elementAt(rowCount, 1)->addNew<Wt::WText>("B" + std::to_string(bend->bend_id));
        bendFeaturesTable->elementAt(rowCount, 2)->addNew<Wt::WText>(processString(bend->bend_angle));
        bendFeaturesTable->elementAt(rowCount, 3)->addNew<Wt::WText>(processString(bend->bend_radius));
        bendFeaturesTable->elementAt(rowCount, 4)->addNew<Wt::WText>(processString(bend->bend_direction));
        bendFeaturesTable->elementAt(rowCount, 5)->addNew<Wt::WText>(processString(bend->bend_length));
        bendFeaturesTable->elementAt(rowCount, 6)->addNew<Wt::WText>(bend->bending_tool_id);

        std::cout << "Angle : " << std::to_string(round(bend->bend_angle)) << std::endl;

        ++rowCount;
        ++size;
    }

    transaction.commit();

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    dialog->show();
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