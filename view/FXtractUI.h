#ifndef FXTRACT_H_
#define FXTRACT_H_

#include <Wt/WContainerWidget.h>

#include "../model/Session.h"

namespace Wt {
    class WStackedWidget;
    class WText;
    class WEnvironment;
    class WTemplate;
}

class ProjectListWidget;
class NavBarWidget;

class FXtractUI : public Wt::WContainerWidget
{
public:
    FXtractUI(const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool);

    void setInternalBasePath(const std::string& basePath);
    void handleInternalPathChange(const std::string &internalPath);

    Wt::WString user();
    void login(const std::string& user);
    void logout();

    Wt::Signal<Wt::WString>& userChanged() { return userChanged_; }

private:
    Session session_;
    std::string basePath_;
    Wt::Signal<Wt::WString> userChanged_;

    Wt::WText *title_;
    NavBarWidget *navBarWidget_;
    ProjectListWidget *projectListContainer_;
    Wt::WContainerWidget *modelViewer_;
    Wt::WStackedWidget    *mainStack_;

    void onAuthEvent();
    void showProjects();
    void showModelViewer(dbo::ptr<Project> project);

    void refresh();
    void loggedIn();
    void loggedOut();
    void displayProjects();
};

#endif