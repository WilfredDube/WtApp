#ifndef PROCESS_PLAN_H
#define PROCESS_PLAN_H

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ModelFile.h"
#include "MachineParam.h"
#include "BendSequence.h"

namespace dbo = Wt::Dbo;

class ModelFile;
class MachineParam;
class BendSequence;

typedef dbo::collection< dbo::ptr<BendSequence> > BendSequences;

class ProcessPlan {
public:
  std::string part_no;
  double stock_size;
  int no_rotations;
  int no_flips;
  int quantity;
  double process_planning_time;
  double estimated_manufacturing_time;
  std::string moderator;
  Wt::WDateTime  dateCreated;

  dbo::ptr<ModelFile> modelFile;
  dbo::weak_ptr<MachineParam> machineParam;
  BendSequences bendSequences;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, part_no, "part_no");
    dbo::field(a, no_rotations, "no_rotations");
    dbo::field(a, no_flips, "no_flips");
    dbo::field(a, stock_size, "stock_size");
    dbo::field(a, process_planning_time, "process_planning_time");
    dbo::field(a, estimated_manufacturing_time, "estimated_manufacturing_time");
    dbo::field(a, moderator, "moderator");
    dbo::field(a, dateCreated, "project_creation_date");

    dbo::belongsTo(a, modelFile, "model_file", dbo::OnDeleteCascade);

    dbo::hasMany(a, bendSequences, dbo::ManyToOne, "bend_sequences");
    dbo::hasOne(a, machineParam, "machine_param");
  }
};

DBO_EXTERN_TEMPLATES(ProcessPlan)

#endif // PROCESS_PLAN_H
