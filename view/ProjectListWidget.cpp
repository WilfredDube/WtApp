#include "ProjectListWidget.h"
#include "ProjectSummaryWidget.h"
#include "../model/Project.h"
#include "../model/Session.h"

#include <Wt/WPushButton.h>
#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WLocalDateTime.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

ProjectListWidget::ProjectListWidget(const std::string& basePath, Session& session, const std::string uploadPath)
  : WContainerWidget(),
  session_(session),
  basePath_(basePath),
  uploadPath_(uploadPath)
{ 
  resize(Wt::WLength::Auto, Wt::WLength::Auto);
  hBox_ = setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

  projectList_ = hBox_->addWidget(Wt::cpp14::make_unique<ProjectList>(), 1);
  projectList_->setScrollVisibilityEnabled(true);
  projectList_->setStyleClass("centered");

  auto createProjectButton = hBox_->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>());
  createProjectButton->addStyleClass("btn btn-floating btn-success btn-circle btn-md");
  createProjectButton->setIcon("icons/add_project.svg");
  createProjectButton->setToolTip("Create New Project");
  createProjectButton->clicked().connect([=] {
      createNewProjectDialog();
  });
}

void ProjectListWidget::createNewProjectDialog()
{
    auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("Create a new project"));

    Wt::WLabel *label = dialog->contents()->addNew<Wt::WLabel>("Project Name");
    Wt::WLineEdit *projectName = dialog->contents()->addNew<Wt::WLineEdit>();
    label->setBuddy(projectName);
    
    Wt::WLabel *label1 = dialog->contents()->addNew<Wt::WLabel>("Description");
    Wt::WTextArea *projectDesc = dialog->contents()->addNew<Wt::WTextArea>();
    label1->setBuddy(projectDesc);

    dialog->contents()->addStyleClass("form-group");

    Wt::WPushButton *ok = dialog->footer()->addNew<Wt::WPushButton>("Save");
    ok->setDefault(true);

    Wt::WPushButton *cancel = dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog->rejectWhenEscapePressed();

    /* Accept the dialog */
    ok->clicked().connect([=] {
            dialog->accept();
    });

    /** Reject the dialog  */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /** Process the dialog result.  */
    dialog->finished().connect([=] {
      if (dialog->result() == Wt::DialogCode::Accepted) {
        dbo::Transaction t(session_);

        dbo::ptr<Project> project(Wt::cpp14::make_unique<Project>());
        Project *p = project.modify();
        p->title = projectName->text();
        p->brief_desc = projectDesc->text();
        p->author = session_.user();
        p->dateCreated = Wt::WDateTime::currentDateTime();

        addProject(project, projectList_);
        
        std::string userFolder = uploadPath_ + "/" + session_.user()->name.toUTF8(); 
        if (fs::exists(userFolder))
        {
          std::string projectFolder = userFolder + "/" + p->titleToUrl();
          fs::create_directories(projectFolder);
          std::cout << "\n***Done : Project folder created >> " + projectFolder << std::endl;
        }

        t.commit();

        projectList_->refresh();
      }
      
      removeChild(dialog);
    });

    dialog->show();
}

void ProjectListWidget::refreshProjects()
{
  projectList_->clear();
}

void ProjectListWidget::showProjects(const Projects projects)
{
  for(auto project : projects)
    addProject(project, projectList_);
}

void ProjectListWidget::addProject(const dbo::ptr<Project> project, Wt::WContainerWidget *parent) 
{
  parent->addWidget(Wt::cpp14::make_unique<ProjectSummaryWidget>(session_, basePath_, project, uploadPath_));
}