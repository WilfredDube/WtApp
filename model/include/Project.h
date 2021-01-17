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

class Project : public dbo::Dbo<Project> 
{
public:
  dbo::ptr<User> author;

  Wt::WString title;
  Wt::WString desc;
  int no_files;
  Wt::WDateTime  date_created;

  ModelFiles cad_files;

  size_t modelCount() const;
  std::string titleToUrl() const;

  template<class Action>
  void persist(Action& a) {
    dbo::field(a, title, "title");
    dbo::field(a, desc, "desc");
    dbo::field(a, no_files, "no_files");
    dbo::field(a, date_created, "date_created");

    dbo::belongsTo(a, author, "author");
    
    dbo::hasMany(a, cad_files, dbo::ManyToOne, "project");
  }
};

DBO_EXTERN_TEMPLATES(Project);

#endif // PROJECT_H_
