#ifndef MACHINE_PARAM_H
#define MACHINE_PARAM_H

#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ProcessPlan.h"

namespace dbo = Wt::Dbo;

class ProcessPlan;

class MachineParam {
public:  
  int num_tools;
  double bending_force;
  
  dbo::ptr<ProcessPlan> processPlan;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, num_tools, "num_tools");
    dbo::field(a, bending_force, "bending_force");

    dbo::belongsTo(a, processPlan, "machine_param", dbo::OnDeleteCascade);
  }
};

DBO_EXTERN_TEMPLATES(MachineParam)

#endif // MACHINE_PARAM_H
