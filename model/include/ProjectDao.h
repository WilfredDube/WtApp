#pragma once

#include "../include/Project.h"
#include "../include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

class ModelFile;
class User;
class Project;

typedef Wt::Dbo::collection< Wt::Dbo::ptr<ModelFile> > ModelFiles;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Project> > Projects;

class ProjectDao
{
public:

    ProjectDao(Session& session) : session { session } {}

    Wt::Dbo::ptr<Project> insert(std::string title, std::string desc);

    Wt::Dbo::ptr<Project> get( std::string title);

    Projects getAll();

    bool deleteProject(Wt::Dbo::ptr<Project>& project);

    ModelFiles getModelFiles(Wt::Dbo::ptr<Project>& project);

private:
    Session& session;
};