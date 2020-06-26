#ifndef PROJECT_SUMMARY_WIDGET_H_
#define PROJECT_SUMMARY_WIDGET_H_

#include <Wt/WContainerWidget.h>
#include <Wt/Dbo/ptr.h>

#include "../model/Project.h"
#include "../model/Session.h"

namespace Wt {
    class WText;
}

class ProjectSummaryWidget : public Wt::WContainerWidget
{
private:
    Session& session_;
    std::string basePath_;
    dbo::ptr<Project> project_;
    std::string uploadPath_;

    Wt::WHBoxLayout *hBox_;
    Wt::WVBoxLayout *vBox_;
    Wt::WImage *projectImage_;
    Wt::WAnchor *projectName_;
    Wt::WText *projectDesc_;
    Wt::WImage *deleteIcon_;
    Wt::WImage *shareIcon_;
    Wt::WImage *openIcon_;

    void render();
public:
    ProjectSummaryWidget(Session& session, const std::string& basePath, dbo::ptr<Project> project, const std::string uploadPath);
    ~ProjectSummaryWidget();

    void buildProject();
    void setProjectImage(const Wt::WLink& projectImage);
    void deleteDialog();
    void rm();
};

#endif 