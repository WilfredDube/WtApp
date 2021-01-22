#pragma once

#include "../include/ProjectSummaryWidget.h"

#include <Wt/WContainerWidget.h>

class ProjectList : public Wt::WContainerWidget
{
public:
  ProjectList();

  void addWidget(std::unique_ptr<ProjectSummaryWidget> projectWidget);

  using WContainerWidget::addWidget;
};