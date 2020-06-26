#include "ProjectList.h"
#include "ProjectSummaryWidget.h"

ProjectList::ProjectList()
  : WContainerWidget()
{ 
}

void ProjectList::addWidget(std::unique_ptr<ProjectSummaryWidget> projectWidget)
{
  Wt::WContainerWidget::addWidget(std::move(projectWidget));
}