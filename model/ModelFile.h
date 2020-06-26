#ifndef MODEL_FILE_H_
#define MODEL_FILE_H_

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
public:
  dbo::ptr<User> author;
  dbo::ptr<Project> project;

  double tensileStrength;
  int nbends;
  int nModules; //number_of_modules;
  Wt::WString modelFile;
  Wt::WString modelObjFile;
  Wt::WString modelFileDir;
  Wt::WDateTime  uploadDate;
  ProcessLevel processLevel;
  Wt::WString modelData;
  Wt::WString modelMaterial;

  BendFeatures bendFeatures;
  dbo::weak_ptr<ProcessPlan> processPlan;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, tensileStrength, "model_tensile_strength");
    dbo::field(a, nbends, "model_bend_count");
    dbo::field(a, nModules, "number_of_modules");
    dbo::field(a, modelFile, "model_file");
    dbo::field(a, modelObjFile, "model_obj_file");
    dbo::field(a, modelFileDir, "model_file_dir");
    dbo::field(a, processLevel, "model_process_level");
    dbo::field(a, uploadDate, "model_upload_date");
    dbo::field(a, modelData, "model_data");
    dbo::field(a, modelMaterial, "model_material");

    dbo::belongsTo(a, project, "project", dbo::OnDeleteCascade);
    dbo::belongsTo(a, author, "author");
    
    dbo::hasMany(a, bendFeatures, dbo::ManyToOne, "bend_features");
    dbo::hasOne(a, processPlan);
  }
};

DBO_EXTERN_TEMPLATES(ModelFile);

#endif // MODEL_FILE_H_
