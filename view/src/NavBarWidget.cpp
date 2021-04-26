#include "../include/NavBarWidget.h"

#include <Wt/WNavigationBar.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WLineEdit.h>

#include "../include/ProjectListWidget.h"

NavBarWidget::NavBarWidget(const std::string title, const std::string link, const std::string& basePath)
    : WContainerWidget(),
        basePath_(basePath),
        title_(title)
{
    navBar_ = addWidget(Wt::cpp14::make_unique<Wt::WNavigationBar>());
    navBar_->setTitle(title_, link);
    navBar_->setStyleClass("nav navbar");
    navBar_->setResponsive(true);

    Wt::WStackedWidget *contentsStack = addNew<Wt::WStackedWidget>();
    contentsStack->addStyleClass("contents");

    // Setup a Left-aligned menu.
    auto leftMenu = Wt::cpp14::make_unique<Wt::WMenu>(contentsStack);
    leftMenu_ = navBar_->addMenu(std::move(leftMenu));

    auto projectViewer = Wt::cpp14::make_unique<Wt::WMenuItem>("Projects");
    projectViewer->setLink(Wt::WLink(Wt::LinkType::InternalPath, basePath_));
    leftMenu_->addItem(std::move(projectViewer));

    // Add a Search control.
    auto editPtr = Wt::cpp14::make_unique<Wt::WLineEdit>();
    auto edit = editPtr.get();
    edit->setPlaceholderText("Search for a project...");
    edit->setMinimumSize(400, 0);
    setLayoutSizeAware(true);

    navBar_->addSearch(std::move(editPtr));
}       

NavBarWidget::~NavBarWidget(){}