#ifndef NAV_BAR_WIDGET_H_
#define NAV_BAR_WIDGET_H_

#include <Wt/WContainerWidget.h>
#include <string>
#include <Wt/WNavigationBar.h>
#include <Wt/WMenu.h>

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

#endif // NAV_BAR_WIDGET_H_