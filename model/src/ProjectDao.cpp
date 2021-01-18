#include "../include/ProjectDao.h"

#include "../include/ModelFile.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Project.h"

#include <memory>

namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<ModelFile> > ModelFiles;

dbo::ptr<Project> ProjectDao::insert(std::string title, std::string desc)
{
    dbo::ptr<Project> project(Wt::cpp14::make_unique<Project>());

    dbo::Transaction t(session);

    Project *p = project.modify();

    p->title = title;
    p->desc = desc;
    p->author = session.user();
    p->date_created = Wt::WDateTime::currentDateTime();

    t.commit();

    return project;
}

dbo::ptr<Project> ProjectDao::get(std::string title)
{
    dbo::ptr<Project> project;

    dbo::Transaction t(session);
    
    project = session.find<Project>()
      .where("title = ?").bind(title);

    t.commit();

    return project;
}

Projects ProjectDao::getAll()
{
    return session.user()->projects;
}

bool ProjectDao::deleteProject(dbo::ptr<Project>& project)
{
    dbo::Transaction t(session);

    project.remove();

    t.commit();

    return true;
}

ModelFiles ProjectDao::getModelFiles(dbo::ptr<Project>& project)
{
    return project->cad_files;
}
