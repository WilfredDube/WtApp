#include "../include/ProcessPlanDialog.h"

#include "../../dbdao/include/ModelFileDao.h"
#include "../../dbdao/include/BendSequenceDao.h"
#include "../../dbdao/include/ProcessPlanDao.h"

#include "../../libseqgen/include/ProductionTime.h"

#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>
#include <Wt/WTable.h>
#include <Wt/WSpinBox.h>
#include <Wt/WInPlaceEdit.h>

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
    download_ = footer()->addNew<Wt::WPushButton>("DOWNLOAD");
    ok_ = footer()->addNew<Wt::WPushButton>("DONE");

    rejectWhenEscapePressed();

    Wt::WTemplate* t = contents()->addWidget(Wt::cpp14::make_unique<Wt::WTemplate>());
    t->setTemplateText(tr("pp.template"));
    setWidth(1200);

    dbo::Transaction transaction(session_);

    auto processPlan = modelFile_->getProcessPlan();
    auto nTools = modelFile_->getProcessPlan()->getNumberOfTools();
    auto nBends = modelFile_->getNumberOfBends();
    auto nFlips = modelFile_->getProcessPlan()->getNumberOfFlips();
    auto nRotations = modelFile_->getProcessPlan()->getNumberOfRotation();
    auto distance = modelFile_->getProcessPlan()->getToolDistance();
    auto bendingForce = modelFile_->getBendingForce();
    auto thickness = modelFile_->getThickness();

    t->bindWidget("process_code", Wt::cpp14::make_unique<Wt::WText>())
                ->setText(modelFile_->getProject()->getTitle());
    
    t->bindWidget("module_no", Wt::cpp14::make_unique<Wt::WText>())
                ->setText(processString(modelFile_->getProcessPlan()->getNumberOfModules()));

    t->bindWidget("planner_name", Wt::cpp14::make_unique<Wt::WText>())
                ->setText(session.user()->name);

    using namespace Fxt::Dao;
    ProcessPlanDao processPlanDao { session };
    std::string placeHolder = modelFile_->getProcessPlan()->getModerator().empty() ? "Click to enter moderator name" : modelFile_->getProcessPlan()->getModerator();
    moderator_ = t->bindWidget("moderator", Wt::cpp14::make_unique<Wt::WInPlaceEdit>(placeHolder));
    moderator_->setPlaceholderText("Enter moderator's name");
    moderator_->valueChanged().connect([&]{        
        processPlanDao.update(modelFile_, moderator_->text());
    });

    creationDate_ = t->bindWidget("creation_date", Wt::cpp14::make_unique<Wt::WText>());
    creationDate_->setText(processPlan->getCreationDate().toString("dddd, MMMM d, yyyy, HH:mm"));

    std::string partPlaceHolder = modelFile_->getProcessPlan()->getPartNo().empty() ? "Click to enter the part number" : modelFile_->getProcessPlan()->getPartNo();
    partNo_ = t->bindWidget("part_no", Wt::cpp14::make_unique<Wt::WInPlaceEdit>(partPlaceHolder));
    partNo_->setPlaceholderText("Enter part number");
    partNo_->valueChanged().connect([&]{
        processPlanDao.update(modelFile_, partNo_->text().toUTF8());
    });

    partName_ = t->bindWidget("part_name", Wt::cpp14::make_unique<Wt::WText>());
    partName_->setText(modelFile_->getCadFileName()); 

    material_ = t->bindWidget("material_name", Wt::cpp14::make_unique<Wt::WText>());
    material_->setText(modelFile_->getModelMaterial());    

    quantity_ = t->bindWidget("quantity", Wt::cpp14::make_unique<Wt::WSpinBox>());
    quantity_->setRange(1,100);

    int value = modelFile_->getProcessPlan()->getQuantity() == 1 ? 1 : modelFile_->getProcessPlan()->getQuantity();
    quantity_->setValue(value);
    quantity_->setSingleStep(1);
    quantity_->setWidth(100);

    nTools_ = t->bindWidget("tools_no", Wt::cpp14::make_unique<Wt::WText>());
    nTools_->setText(processString(nTools));

    bendingForce_ = t->bindWidget("bending_force", Wt::cpp14::make_unique<Wt::WText>());
    bendingForce_->setText(processString(bendingForce));

    nRotations_ = t->bindWidget("rotations_no", Wt::cpp14::make_unique<Wt::WText>());
    nRotations_->setText(processString(nRotations));

    nFlips_ = t->bindWidget("flips_no", Wt::cpp14::make_unique<Wt::WText>());
    nFlips_->setText(processString(nFlips));

    processPlanningTime_ = t->bindWidget("process_planning_time", Wt::cpp14::make_unique<Wt::WText>());
    processPlanningTime_->setText(processString(processPlan->getProcessPlanningTime()));

    totalProcessingTime_ = t->bindWidget("total_processing_time", Wt::cpp14::make_unique<Wt::WText>());
    auto totalProductionTime = computeTotalProductionTime(quantity_->value(), nTools, nBends, nFlips, nRotations);
    totalProcessingTime_->setText(processString(totalProductionTime));

    quantity_->changed().connect([=]{
        quantityChanged(processPlanDao, quantity_->value(), nTools, nBends, nFlips, nRotations);
    });

    quantity_->enterPressed().connect([=]{
        quantityChanged(processPlanDao, quantity_->value(), nTools, nBends, nFlips, nRotations);
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

    BendFeatures bf = modelFile_->getBendFeatures();

    int rowCount = bendSequenceTable_->rowCount();

    auto bendingSequence = processPlan->getBendingSequence();

    // std::cout << "Sequence size: " + bendingSequence.size() << std::endl;

    // auto startIter = bendingSequence.begin();
    // auto endIter = bendingSequence.end();

    int size = 1;
    for(auto startIter = bendingSequence.begin(); startIter != bendingSequence.end(); ++startIter)
    {    
        for(auto& bend : bf) 
        {
            if (bend->bend_id == startIter->get()->getBendId() )
            {
                std::cout << "Bend Sequences count : " << startIter->get()->getBendId() << std::endl;

                bendSequenceTable_->elementAt(rowCount, 0)->addNew<Wt::WText>(std::to_string(size));
                bendSequenceTable_->elementAt(rowCount, 1)->addNew<Wt::WText>("B" + std::to_string(bend->bend_id));
                bendSequenceTable_->elementAt(rowCount, 2)->addNew<Wt::WText>(processString(bend->bend_angle));
                bendSequenceTable_->elementAt(rowCount, 3)->addNew<Wt::WText>(processString(bend->bend_radius));

                std::string bendDirection;
                if(bend->bend_direction == 1)
                    bendDirection = "INSIDE";
                else if(bend->bend_direction == 2)
                    bendDirection = "OUTSIDE";

                bendSequenceTable_->elementAt(rowCount, 4)->addNew<Wt::WText>(bendDirection);
                
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

inline void ProcessPlanDialog::quantityChanged(Fxt::Dao::ProcessPlanDao processPlanDao, unsigned nParts, unsigned nTools, unsigned nBends, unsigned nFlips, unsigned nRotations)
{
    auto time_p = computeTotalProductionTime(quantity_->value(), nTools, nBends, nFlips, nRotations);;
    totalProcessingTime_->setText(processString(time_p));

    processPlanDao.update(modelFile_, quantity_->value());
}

inline void ProcessPlanDialog::setProductionTime(const double time_p)
{
    std::cout << "+++++++++++++++++++_++++++++++++++++++++" << std::endl;
    std::cout << "Seconds = " << time_p << std::endl;
    std::cout << "+++++++++++++++++++_++++++++++++++++++++" << std::endl;
    auto time = std::chrono::seconds(static_cast<int>(time_p));
    
    auto days = std::chrono::duration_cast<std::chrono::duration<int,std::ratio<3600*24>>>(time);
    auto hours = std::chrono::duration_cast<std::chrono::hours>(time  % std::chrono::hours(24));
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(time % std::chrono::hours(1));;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(time % std::chrono::minutes(1));

    totalProcessingTime_->setText(std::to_string(days.count()) + " d : " + std::to_string(hours.count()) + " h : " + std::to_string(mins.count()) + " min :" + (std::to_string(secs.count()) + " s"));
}

ProcessPlanDialog::~ProcessPlanDialog() {}