#pragma once

#include "../../model/include/ProcessPlan.h"
#include "../../model/include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<ProcessPlan> > ProcessPlans;

class BendSequenceGenerator;
class ModelFile;

namespace Fxt 
{
namespace Dao
{
    class ProcessPlanDao
    {
    public:

        ProcessPlanDao(Session& session) : session { session } {}

        Wt::Dbo::ptr<ProcessPlan> insert(BendSequenceGenerator& bendSequence,
                                        dbo::ptr<ModelFile>& modelFile,
                                        double process_planning_time,
                                        std::string moderator = "");

        Wt::Dbo::ptr<ProcessPlan> update(dbo::ptr<ModelFile>& modelFile, size_t quantity);
        Wt::Dbo::ptr<ProcessPlan> update(dbo::ptr<ModelFile>& modelFile, const Wt::WString& partNumber);
        Wt::Dbo::ptr<ProcessPlan> updateModerator(dbo::ptr<ModelFile>& modelFile, const Wt::WString& moderator);
        Wt::Dbo::ptr<ProcessPlan> update(BendSequences bendSequences);
        Wt::Dbo::ptr<ProcessPlan> update(dbo::ptr<ModelFile>& modelFile, double estimated_manufacturing_time);
        Wt::Dbo::ptr<ProcessPlan> update(dbo::ptr<ModelFile>& modelFile, int numberOfModules);

        Wt::Dbo::ptr<ProcessPlan> get(dbo::ptr<ModelFile>& modelFile);

        ProcessPlans getAll();

        bool deleteProject(Wt::Dbo::ptr<ProcessPlan>& processPlan);

    private:
        Session& session;
    };
}
}