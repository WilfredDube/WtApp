#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WMenu.h>

#include <string>

class NavBarWidget : public Wt::WContainerWidget
{
private:
    std::string title_,  basePath_;
    Wt::WNavigationBar *navBar_;
    Wt::WMenu *leftMenu_;
    Wt::WMenu *rightMenu_;
public:
    NavBarWidget(const std::string title, const std::string link, const std::string& basePath);
    ~NavBarWidget();
};