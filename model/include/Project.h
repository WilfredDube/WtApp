#ifndef PROJECT_H_
#define PROJECT_H_

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ModelFile.h"

class User;
class ModelFile;

namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<ModelFile> > ModelFiles;

class Project : public dbo::Dbo<Project> {
public:
  dbo::ptr<User> author;

  Wt::WString title;
  Wt::WString brief_desc;
  int nModelFiles;
  Wt::WDateTime  dateCreated;

  ModelFiles modelFiles;

  size_t modelCount() const;
  std::string titleToUrl() const;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, title, "project_title");
    dbo::field(a, brief_desc, "project_brief_desc");
    dbo::field(a, nModelFiles, "project_model_count");
    dbo::field(a, dateCreated, "project_creation_date");

    dbo::belongsTo(a, author, "author");
    
    dbo::hasMany(a, modelFiles, dbo::ManyToOne, "project");
  }
};

DBO_EXTERN_TEMPLATES(Project);

#endif // PROJECT_H_
