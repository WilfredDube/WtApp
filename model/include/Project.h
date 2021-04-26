#pragma once

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "ModelFile.h"

class User;
class ModelFile;

namespace dbo = Wt::Dbo;

namespace Fxt 
{
namespace Dao
{
  class ProjectDao;
  class ModelFileDao;
}
}

typedef dbo::collection< dbo::ptr<ModelFile> > ModelFiles;

class Project : public dbo::Dbo<Project> 
{
  dbo::ptr<User> author;

  Wt::WString title;
  Wt::WString desc;
  int no_files;
  Wt::WDateTime  date_created;

  ModelFiles cad_files;

public:
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

  dbo::ptr<User> getAuthor() const { return author; }

  Wt::WString getTitle() const { return title; }
  Wt::WString getDescription() const { return desc; }
  int getNumberOfFiles() const { return no_files; }
  Wt::WDateTime getCreationDate() const { return date_created; }

  ModelFiles getModelFiles() const { return cad_files; }

  friend class Fxt::Dao::ProjectDao;
  friend class Fxt::Dao::ModelFileDao;
};

DBO_EXTERN_TEMPLATES(Project);
