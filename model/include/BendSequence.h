#pragma once

#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ProcessPlan.h"

namespace dbo = Wt::Dbo;

namespace Fxt 
{
namespace Dao
{
  class BendSequenceDao;
}
}

class ProcessPlan;

class BendSequence 
{
  int bend_id;
  dbo::ptr<ProcessPlan> processPlan;

public:
  template<class Action>
  void persist(Action& a) {
    dbo::field(a, bend_id, "bend_id");

    dbo::belongsTo(a, processPlan, "process_plan", dbo::OnDeleteCascade);
  }

  int getBendId() const { return bend_id; }
  dbo::ptr<ProcessPlan> getProcessPlan() const { return processPlan; }
  
  friend class Fxt::Dao::BendSequenceDao;
};

DBO_EXTERN_TEMPLATES(BendSequence)
