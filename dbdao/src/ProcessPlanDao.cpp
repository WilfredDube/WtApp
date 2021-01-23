#include "../include/ProcessPlanDao.h"

#include "../../libseqgen/include/BendSequenceGenerator.h"

#include <memory>

namespace dbo = Wt::Dbo;

using namespace Fxt::Dao;

Wt::Dbo::ptr<ProcessPlan> ProcessPlanDao::insert(BendSequenceGenerator& bendSequence,
                                     dbo::ptr<ModelFile>& modelFile,
                                     double process_planning_time,
                                     std::string moderator)

{
    auto ret = Wt::Dbo::ptr<ProcessPlan>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<ProcessPlan> processPlan(Wt::cpp14::make_unique<ProcessPlan>());
        processPlan->process_planning_time = process_planning_time;
        processPlan->moderator = moderator;
        processPlan->modelFile = modelFile;

        processPlan->no_flips = bendSequence.getNumberOfFlips();
        processPlan->no_rotations = bendSequence.getNumberOfRotations();
        processPlan->no_tools = bendSequence.getNumberOfTools();
        processPlan->tool_distance = bendSequence.getNumberOfDistance();

        processPlan->process_planning_time += process_planning_time;
        processPlan->dateCreated = Wt::WDateTime::currentDateTime();

        processPlan->quantity = 1;
        
        ret = session.add(std::move(processPlan));
    }
    
    return ret;
}

Wt::Dbo::ptr<ProcessPlan> ProcessPlanDao::update(dbo::ptr<ModelFile>& modelFile, const Wt::WString& moderator)
{
    {
        dbo::Transaction transaction(session);

        modelFile->processPlan.modify()->moderator = moderator.toUTF8();
    }

    return  modelFile->processPlan;
}

Wt::Dbo::ptr<ProcessPlan> ProcessPlanDao::update(dbo::ptr<ModelFile>& modelFile, std::string& partNumber)
{
    {
        dbo::Transaction transaction(session);

        modelFile->processPlan.modify()->part_no = partNumber;
    }

    return  modelFile->processPlan;
}

Wt::Dbo::ptr<ProcessPlan> ProcessPlanDao::update(dbo::ptr<ModelFile>& modelFile, size_t quantity)
{
    {
        dbo::Transaction transaction(session);

        modelFile->processPlan.modify()->quantity = quantity;
    }

    return  modelFile->processPlan;
}


dbo::ptr<ProcessPlan> ProcessPlanDao::update(dbo::ptr<ModelFile>& modelFile, double manufacturing_time)
{
    {
        dbo::Transaction transaction(session);

         modelFile->processPlan.modify()->estimated_manufacturing_time = manufacturing_time;
    }

    return  modelFile->processPlan;
}

dbo::ptr<ProcessPlan> ProcessPlanDao::update(dbo::ptr<ModelFile>& modelFile, int numberOfModules)
{
    {
        dbo::Transaction transaction(session);

         modelFile->processPlan.modify()->no_modules = numberOfModules;
    }

    return  modelFile->processPlan;
}

dbo::ptr<ProcessPlan> ProcessPlanDao::get(dbo::ptr<ModelFile>& modelFile)
{
    dbo::Transaction transaction(session);
    
    dbo::ptr<ProcessPlan> processPlan = session.query<dbo::ptr<ProcessPlan>>("select p from process_plan p")
                                        .where("modelFile = ?").bind(modelFile);

    return processPlan;
}

ProcessPlans ProcessPlanDao::getAll()
{
    return session.find<ProcessPlan>();
}

bool ProcessPlanDao::deleteProject(dbo::ptr<ProcessPlan>& processPlan)
{
    dbo::Transaction transaction(session);

    processPlan.remove();

    return true;
}