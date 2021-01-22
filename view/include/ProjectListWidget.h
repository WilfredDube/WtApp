#pragma once

#include "../include/ProjectList.h"

#include <Wt/WContainerWidget.h>

namespace Wt {
  class WLink;
  class WHBoxLayout;
  class WString;
}

class Session;

class ProjectListWidget : public Wt::WContainerWidget
{
public:
  ProjectListWidget(const std::string& basePath, Session& session, const std::string uploadPath);

  void addProject(const Wt::WString& text, const Wt::WString& desc, const Wt::WLink& img);
  void showProjects(const Projects projects);

  void refreshProjects();

  using WContainerWidget::addWidget;

private:
  Session& session_;
  std::string basePath_;
  std::string uploadPath_;

  Wt::WHBoxLayout *hBox_;
  ProjectList *projectList_;
  Wt::WPushButton *addNewProjectButton_;
  
  void createNewProjectDialog();
  void addProject(const dbo::ptr<Project> post, Wt::WContainerWidget *parent);
};