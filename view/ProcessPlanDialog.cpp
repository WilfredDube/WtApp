#include "ProcessPlanDialog.h"

#include "../model/ModelFile.h"
#include "../model/BendSequence.h"
#include "../libSeqgen/ProductionTime.h"

#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WTable.h>
#include <Wt/WSpinBox.h>
#include <Wt/WInPlaceEdit.h>
#include <Wt/WMessageBox.h>

namespace {

    auto processString = [](double x) {
        std::ostringstream os;
        os << x;
        return os.str();
    };
}

ProcessPlanDialog::ProcessPlanDialog(Session& session, const std::string& title, Wt::Dbo::ptr<ModelFile>& modelFile)
    : Wt::WDialog(title),
    session_(session),
    modelFile_(modelFile)
{
    auto& processPlan = modelFile_->processPlan;

    download_ = footer()->addNew<Wt::WPushButton>("DOWNLOAD");
    ok_ = footer()->addNew<Wt::WPushButton>("DONE");

    rejectWhenEscapePressed();

    Wt::WTemplate* t = contents()->addWidget(Wt::cpp14::make_unique<Wt::WTemplate>());
    t->setTemplateText(tr("pp.template"));
    setWidth(1200);

    dbo::Transaction transaction(session_);

    t->bindWidget("process_code", Wt::cpp14::make_unique<Wt::WText>())
                ->setText(modelFile_->project->title);
    
    t->bindWidget("module_no", Wt::cpp14::make_unique<Wt::WText>())
                ->setText(processString(modelFile_->nModules));

    t->bindWidget("planner_name", Wt::cpp14::make_unique<Wt::WText>())
                ->setText(modelFile_->author->name);

    std::string placeHolder = modelFile_->processPlan->moderator.empty() ? "Click to enter moderator name" : modelFile_->processPlan->moderator;
    moderator_ = t->bindWidget("moderator", Wt::cpp14::make_unique<Wt::WInPlaceEdit>(placeHolder));
    moderator_->setPlaceholderText("Enter moderator's name");
    moderator_->valueChanged().connect([&]{
        dbo::Transaction trn(session);
        modelFile_->processPlan.modify()->moderator = moderator_->text().toUTF8();
        trn.commit();
    });

    creationDate_ = t->bindWidget("creation_date", Wt::cpp14::make_unique<Wt::WText>());
    creationDate_->setText(processPlan->dateCreated.toString("dddd, MMMM d, yyyy, HH:mm"));

    std::string partPlaceHolder = modelFile_->processPlan->part_no.empty() ? "Click to enter the part number" : modelFile_->processPlan->part_no;
    partNo_ = t->bindWidget("part_no", Wt::cpp14::make_unique<Wt::WInPlaceEdit>(partPlaceHolder));
    partNo_->setPlaceholderText("Enter part number");
    partNo_->valueChanged().connect([&]{
        dbo::Transaction trn(session);
        modelFile_->processPlan.modify()->part_no = partNo_->text().toUTF8();
        trn.commit();
    });

    partName_ = t->bindWidget("part_name", Wt::cpp14::make_unique<Wt::WText>());
    partName_->setText(modelFile_->modelFile); 

    material_ = t->bindWidget("material_name", Wt::cpp14::make_unique<Wt::WText>());
    material_->setText(modelFile_->modelMaterial);    

    quantity_ = t->bindWidget("quantity", Wt::cpp14::make_unique<Wt::WSpinBox>());
    quantity_->setRange(1,100);
    int value = modelFile_->processPlan->quantity == 1 ? 1 : modelFile_->processPlan->quantity;
    quantity_->setValue(value);
    quantity_->setSingleStep(1);
    quantity_->setWidth(100);
    quantity_->changed().connect([&]{
        dbo::Transaction trn(session);
        modelFile_->processPlan.modify()->quantity = quantity_->value();
        trn.commit();
    });

    nTools_ = t->bindWidget("tools_no", Wt::cpp14::make_unique<Wt::WText>());
    nTools_->setText(processString(modelFile_->processPlan->machineParam->num_tools));

    bendingForce_ = t->bindWidget("bending_force", Wt::cpp14::make_unique<Wt::WText>());
    bendingForce_->setText(processString(modelFile_->bendingForce));

    nRotations_ = t->bindWidget("rotations_no", Wt::cpp14::make_unique<Wt::WText>());
    nRotations_->setText(processString(processPlan->no_rotations));

    nFlips_ = t->bindWidget("flips_no", Wt::cpp14::make_unique<Wt::WText>());
    nFlips_->setText(processString(processPlan->no_flips));

    processPlanningTime_ = t->bindWidget("process_planning_time", Wt::cpp14::make_unique<Wt::WText>());
    processPlanningTime_->setText(processString(processPlan->process_planning_time));

    totalProcessingTime_ = t->bindWidget("total_processing_time", Wt::cpp14::make_unique<Wt::WText>());
    totalProcessingTime_->setText(processString(processPlan->estimated_manufacturing_time));

    quantity_->changed().connect([&]{
        dbo::Transaction trn(session);
        modelFile_->processPlan.modify()->quantity = quantity_->value();

        auto time_p = computeTotalTime(quantity_->value(), nTools, nBend, nFlips, nRotations, distance, bendingForce);
        totalProcessingTime_->setText(std::to_string(time_p));
        
        trn.commit();
    });

    bendSequenceTable_  = t->bindWidget("feature_table", Wt::cpp14::make_unique<Wt::WTable>());

    bendSequenceTable_->setWidth(Wt::WLength("100%"));

    bendSequenceTable_->elementAt(0, 0)->addNew<Wt::WText>("<b>OPERATION NO</b>");
    bendSequenceTable_->elementAt(0, 1)->addNew<Wt::WText>("<b>BEND ID</b>");
    bendSequenceTable_->elementAt(0, 2)->addNew<Wt::WText>("<b>BEND ANGLE</b>");
    bendSequenceTable_->elementAt(0, 3)->addNew<Wt::WText>("<b>BEND RADIUS</b>");
    bendSequenceTable_->elementAt(0, 4)->addNew<Wt::WText>("<b>BEND DIRECTION</b>");
    bendSequenceTable_->elementAt(0, 5)->addNew<Wt::WText>("<b>BEND LENGTH</b>");
    bendSequenceTable_->elementAt(0, 6)->addNew<Wt::WText>("<b>TOOLS</b>");

    BendFeatures bf = modelFile_->bendFeatures;

    int rowCount = bendSequenceTable_->rowCount();

    auto& seq = processPlan->bendSequences;

    auto startIter = processPlan->bendSequences.begin();
    auto endIter = processPlan->bendSequences.end();

    int size = 1;
    for( ; startIter != endIter; ++startIter)
    {    
        for(auto& bend : bf) 
        {
            if (bend->bend_id == startIter->get()->bend_id )
            {
                std::cout << "Bend Sequences count : " << startIter->get()->bend_id << std::endl;

                bendSequenceTable_->elementAt(rowCount, 0)->addNew<Wt::WText>(std::to_string(size));
                bendSequenceTable_->elementAt(rowCount, 1)->addNew<Wt::WText>("B" + std::to_string(bend->bend_id));
                bendSequenceTable_->elementAt(rowCount, 2)->addNew<Wt::WText>(processString(bend->bend_angle));
                bendSequenceTable_->elementAt(rowCount, 3)->addNew<Wt::WText>(processString(bend->bend_radius));
                bendSequenceTable_->elementAt(rowCount, 4)->addNew<Wt::WText>(processString(bend->bend_direction));
                bendSequenceTable_->elementAt(rowCount, 5)->addNew<Wt::WText>(processString(bend->bend_length));
                bendSequenceTable_->elementAt(rowCount, 6)->addNew<Wt::WText>(bend->bending_tool_id);

                ++rowCount;
                ++size;
            }            
        }
    }

    transaction.commit();

    /*
     * Reject the dialog
     */
    ok_->clicked().connect(this, &Wt::WDialog::reject);

    // session_.modelFileClicked().connect(this, &ProcessPlanDialog::setModelCrumb);
}

void ProcessPlanDialog::setModelCrumb(dbo::ptr<ModelFile> modelFile)
{
    modelFile_ = modelFile;
    refresh();
}

ProcessPlanDialog::~ProcessPlanDialog(){}