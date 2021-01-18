#include "../include/ProjectDao.h"

#include "../../model/include/ModelFile.h"
#include "../../model/include/User.h"

#include <memory>

namespace dbo = Wt::Dbo;

Wt::Dbo::ptr<Project> ProjectDao::insert(std::string title, std::string desc)
{
    auto ret = Wt::Dbo::ptr<Project>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<Project> project(Wt::cpp14::make_unique<Project>());
        project->title = title;
        project->desc = desc;
        project->author = session.user();
        project->date_created = Wt::WDateTime::currentDateTime();

        ret = session.add(std::move(project));
    }
    
    return ret;
}

dbo::ptr<Project> ProjectDao::get(std::string title)
{
    dbo::Transaction t(session);
    
    dbo::ptr<Project> project = session.query<dbo::ptr<Project>>("select p from project p")
                                        .where("title = ?").bind(title)
                                        .where("author_id = ?").bind(session.user());

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
