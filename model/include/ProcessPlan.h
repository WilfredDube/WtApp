#pragma once

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ModelFile.h"
#include "BendSequence.h"

namespace dbo = Wt::Dbo;

class ModelFile;
class MachineParam;
class BendSequence;

typedef dbo::collection< dbo::ptr<BendSequence> > BendSequences;

class ProcessPlan 
{
class ProcessPlan {
public:
  std::string part_no;
  int no_rotations;
  int no_flips;
  int no_tools;
  int no_modules;
  int quantity;
  double tool_distance;
  double process_planning_time;
  double estimated_manufacturing_time;
  std::string moderator;
  Wt::WDateTime  dateCreated;

  dbo::ptr<ModelFile> modelFile;
  BendSequences bendSequences;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, part_no, "part_no");
    dbo::field(a, no_rotations, "no_rotations");
    dbo::field(a, no_flips, "no_flips");
    dbo::field(a, no_modules, "no_modules");
    dbo::field(a, quantity, "quantity");
    dbo::field(a, process_planning_time, "process_planning_time");
    dbo::field(a, estimated_manufacturing_time, "estimated_manufacturing_time");
    dbo::field(a, tool_distance, "tool_distance");
    dbo::field(a, moderator, "moderator");
    dbo::field(a, dateCreated, "project_creation_date");

    dbo::belongsTo(a, modelFile, "model_file", dbo::OnDeleteCascade);

    dbo::hasMany(a, bendSequences, dbo::ManyToOne, "process_plan");
  }
};

DBO_EXTERN_TEMPLATES(ProcessPlan)