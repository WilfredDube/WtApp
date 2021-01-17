#pragma once

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

class ModelFile;
class Session;
class User;
class Project;

typedef Wt::Dbo::collection< Wt::Dbo::ptr<ModelFile> > ModelFiles;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Project> > Projects;

class ProjectDao
{
public:

    ProjectDao(Session& session) : session { session }, projectTable { session.user()->projects } {}

    Wt::Dbo::ptr<Project> insert(std::string title, std::string desc);

    Wt::Dbo::ptr<Project> getProject( std::string title);

    Projects getAllProjects();

    bool deleteProject(Wt::Dbo::ptr<Project>& project);

    ModelFiles getProjectModelFiles(dbo::ptr<Project>& project);

private:
    Projects projectTable;
    Session& session;
};