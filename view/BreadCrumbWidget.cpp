#include "BreadCrumbWidget.h"
#include "../model/ModelFile.h"

#include <Wt/WText.h>

BreadCrumbWidget::BreadCrumbWidget(Session& session, const std::string project, const std::string model)
    : WContainerWidget(),
    session_(session)
{
    project_ = addWidget(Wt::cpp14::make_unique<Wt::WText>(project));
    project_->setInline(true);

    addWidget(Wt::cpp14::make_unique<Wt::WText>("<b> > </b>"))->setInline(true);

    model_ = addWidget(Wt::cpp14::make_unique<Wt::WText>(model));
    model_->setInline(true);

    setWidth(Wt::WLength::Auto);
    addStyleClass("color-style", true);
    setMargin(0);

    session_.modelFileClicked().connect(this, &BreadCrumbWidget::setModelCrumb);
}

void BreadCrumbWidget::setModelCrumb(dbo::ptr<ModelFile> model)
{
    model_->setText(model->modelFile);
    refresh();
}

BreadCrumbWidget::~BreadCrumbWidget(){}