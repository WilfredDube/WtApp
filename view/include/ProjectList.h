/*
 * Copyright (C) 2020 Wilfred Dube
 *
 * See the LICENSE file for terms of use.
 */
#ifndef PROJECT_LIST_H_
#define PROJECT_LIST_H_

#include <Wt/WContainerWidget.h>

#include "../include/ProjectSummaryWidget.h"

class ProjectList : public Wt::WContainerWidget
{
public:
  ProjectList();

  void addWidget(std::unique_ptr<ProjectSummaryWidget> projectWidget);

  using WContainerWidget::addWidget;
};

#endif // PROJECT_LIST_H_