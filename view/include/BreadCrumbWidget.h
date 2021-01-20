#ifndef BREAD_CRUMB_WIDGET_H_
#define BREAD_CRUMB_WIDGET_H_

#include <Wt/WContainerWidget.h>
#include "../../model/include/Session.h"

class ModelFile;

namespace Wt {
    class WText;
}

class BreadCrumbWidget : public Wt::WContainerWidget
{
private:
    Session& session_;
    Wt::WText* project_;
    Wt::WText* model_;
public:
    void setModelCrumb(dbo::ptr<ModelFile> modelFile);
    BreadCrumbWidget(Session& session, const std::string project, const std::string model = std::string());
    ~BreadCrumbWidget();
};

#endif