#pragma once

#include "../../model/include/Session.h"

#include "../../libfxtract/include/sheet-metal-component/ModelTypes.h"

#include <Wt/WContainerWidget.h>

namespace Wt {
  class WText;
  class WHBoxLayout;
  class WString;
  class WImage;
}

class ModelFile;
class ColorWidget;

class ModelFileWidget : public Wt::WContainerWidget
{
private:
    Session& session_;
    dbo::ptr<ModelFile> modelFile_;

    Wt::WHBoxLayout *hBox_;
    ColorWidget *icons_;
    Wt::WText *modelFileName_;
    Wt::WImage *deleteIcon_;

    void processModelFile();
    void deleteModelFile();
    void rm();
public:
    ModelFileWidget(Session& session, dbo::ptr<ModelFile> model);
    ~ModelFileWidget();
};