#pragma once

#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ModelFile.h"
#include "Tool.h"

namespace dbo = Wt::Dbo;

class ModelFile;

typedef dbo::collection< dbo::ptr<Tool> > Tools;

class BendFeature 
{
public:
  int bend_id;
  int face_id1;
  int face_id2;
  double bend_angle;
  double bend_radius;
  double bend_force;
  double bend_length;
  int bend_direction;
  std::string bending_tool_id;

  dbo::ptr<ModelFile> modelFile;

public:
  template<class Action>
  void persist(Action& a) {
    dbo::field(a, bend_id, "bend_id");
    dbo::field(a, face_id1, "face_id1");
    dbo::field(a, face_id2, "face_id2");
    dbo::field(a, bend_angle, "bend_angle");
    dbo::field(a, bend_radius, "bend_radius");
    dbo::field(a, bend_force, "bend_force");
    dbo::field(a, bend_length, "bend_length");
    dbo::field(a, bend_direction, "bend_direction");
    dbo::field(a, bending_tool_id, "bending_tool_id");

    dbo::belongsTo(a, modelFile, "bend_features", dbo::OnDeleteCascade);
  }

  int getBendId() const { return bend_id; }
  int getFaceID1() const { return face_id1; }
  int getFaceID2() const { return face_id2; }

  double getAngle() const { return bend_angle; }
  double getRadius() const { return bend_radius; }
  double getForce() const { return bend_force; }
  double getLength() const { return bend_length; }

  int getBendDirection() const { return bend_direction; }
  std::string getBendToolId() const { return bending_tool_id; }

  dbo::ptr<ModelFile> getModelFile() const { return modelFile; }

  friend class BendFeatureDao;
};

DBO_EXTERN_TEMPLATES(BendFeature)
