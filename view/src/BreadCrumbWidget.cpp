#include "../include/BreadCrumbWidget.h"

#include "../../dbdao/include/ModelFileDao.h"

#include <Wt/WText.h>

BreadCrumbWidget::BreadCrumbWidget(Session& session, const std::string project, const std::string model)
    : WContainerWidget(),
    session_(session)
{
    projectNameTxt = addWidget(Wt::cpp14::make_unique<Wt::WText>(project));
    projectNameTxt->setInline(true);

    addWidget(Wt::cpp14::make_unique<Wt::WText>("<b> > </b>"))->setInline(true);

    modelNameTxt = addWidget(Wt::cpp14::make_unique<Wt::WText>(model));
    modelNameTxt->setInline(true);

    setWidth(Wt::WLength::Auto);
    addStyleClass("color-style", true);
    setMargin(0);

    session_.modelFileClicked().connect(this, &BreadCrumbWidget::setModelCrumb);
}

void BreadCrumbWidget::setModelCrumb(dbo::ptr<ModelFile> model)
{
    modelNameTxt->setText(model->getCadFileName());
    refresh();
}

BreadCrumbWidget::~BreadCrumbWidget(){}