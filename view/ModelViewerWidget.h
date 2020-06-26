#ifndef MODEL_VIEWER_WIDGET_H_
#define MODEL_VIEWER_WIDGET_H_

#include <Wt/WContainerWidget.h>
#include "../model/Project.h"
#include "../model/Session.h"

namespace Wt {
    class WPushButton;
    class WGridLayout;
    class WEnvironment;
}

class ModelViewerControls;
class PaintWidget;
class ModelInfoPanels;
class BreadCrumbWidget;
class ModelFile;

class ModelViewerWidget : public Wt::WContainerWidget
{
private:
    Session& session_;
    dbo::ptr<Project> project_;
    std::string uploadPath_;

    BreadCrumbWidget *breadCrumb_;
    ModelInfoPanels *modelInfoPanels_;
    PaintWidget *paintWidget_;
    ModelViewerControls *controls_;
    Wt::WGridLayout *hbox_;
    Wt::WContainerWidget *paintWidgetContainer_;

    void clearWebGL();
    void updateShaders(dbo::ptr<ModelFile> modelFile);
    void displayWebGL(std::vector<float> data);

    void setBendDirection(dbo::ptr<ModelFile> modelFile);
public:
    ModelViewerWidget(Session& session, dbo::ptr<Project> project, const std::string uploadPath);
    ~ModelViewerWidget();
};

#endif