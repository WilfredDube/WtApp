#pragma once

#include "../../model/include/Session.h"

#include <Wt/WContainerWidget.h>

class ModelFile;

namespace Wt {
    class WText;
}

class BreadCrumbWidget : public Wt::WContainerWidget
{
private:
    Session& session_;
    Wt::WText* projectNameTxt;
    Wt::WText* modelNameTxt;
public:
    void setModelCrumb(dbo::ptr<ModelFile> modelFile);
    BreadCrumbWidget(Session& session, const std::string project, const std::string model = std::string());
    ~BreadCrumbWidget();
};