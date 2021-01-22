#pragma once

#include <Wt/WDateTime.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include "ProcessPlan.h"
#include "BendFeature.h"
#include "Project.h"
#include "User.h"

class User;
class Project;
class BendFeature;
class ProcessPlan;

namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<BendFeature> > BendFeatures;

enum class ProcessLevel { UNPROCESSED, FEATURE_EXTRACTED, PROCESS_PLAN_GEN };

class ModelFile {
  dbo::ptr<User> author;
  dbo::ptr<Project> project;

  double bendingForce, feature_recognition_time, thickness;
  int nbends;
  Wt::WString modelFile;
  Wt::WString modelObjFile;
  Wt::WString modelFileDir;
  Wt::WDateTime  uploadDate;
  ProcessLevel processLevel;
  Wt::WString modelData;
  Wt::WString modelMaterial;

  BendFeatures bendFeatures;
  dbo::weak_ptr<ProcessPlan> processPlan;

public:
  template<class Action>
  void persist(Action& a) {
    dbo::field(a, bendingForce, "bending_force");
    dbo::field(a, nbends, "model_bend_count");
    dbo::field(a, modelFile, "model_file");
    dbo::field(a, modelObjFile, "model_obj_file");
    dbo::field(a, modelFileDir, "model_file_dir");
    dbo::field(a, processLevel, "model_process_level");
    dbo::field(a, uploadDate, "model_upload_date");
    dbo::field(a, modelData, "model_data");
    dbo::field(a, modelMaterial, "model_material");
    dbo::field(a, thickness, "thickness");
    dbo::field(a, feature_recognition_time, "feature_recognition_time");

    dbo::belongsTo(a, project, "project", dbo::OnDeleteCascade);
    dbo::belongsTo(a, author, "author");
    
    dbo::hasMany(a, bendFeatures, dbo::ManyToOne, "bend_features");
    dbo::hasOne(a, processPlan);
  }

  dbo::ptr<User> getAuthor() { return author; }
  dbo::ptr<Project> getProject() { return project; }

  double getBendingForce() const { return bendingForce; }
  double getFeatureRecognitionTime() const { return feature_recognition_time; }
  double getThickness() const { return thickness; }
  double getNumberOfBends() const { return nbends; }

  Wt::WString getCadFileName() const { return modelFile; }
  Wt::WString getModelObjFile() const { return modelObjFile; }
  Wt::WString getCadFileDir() const { return modelFileDir; }
  Wt::WString getModelData() const { return modelData; }
  Wt::WString getModelMaterial() const { return modelMaterial; }

  ProcessLevel getProcessLevel() const { return processLevel; }
  BendFeatures getBendFeatures() const { return bendFeatures; }
  dbo::weak_ptr<ProcessPlan> getProcessPlan() const { return processPlan; }

  friend class BendFeatureDao;
  friend class BendSequenceDao;
  friend class ModelFileDao;
};

DBO_EXTERN_TEMPLATES(ModelFile);