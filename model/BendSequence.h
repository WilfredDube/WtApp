#ifndef BENDING_SEQUENCE_H
#define BENDING_SEQUENCE_H

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ProcessPlan.h"

namespace dbo = Wt::Dbo;

class ProcessPlan;

class BendSequence {
public:
  int bend_id, test;
  dbo::ptr<ProcessPlan> processPlan;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, bend_id, "bend_id");
    dbo::field(a, test, "test");

    dbo::belongsTo(a, processPlan, "bend_sequences", dbo::OnDeleteCascade);
  }
};

DBO_EXTERN_TEMPLATES(BendSequence)

#endif // BENDING_SEQUENCE_H
