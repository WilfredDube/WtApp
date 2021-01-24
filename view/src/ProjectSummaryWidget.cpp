#include "../include/ProjectSummaryWidget.h"

#include <memory>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WText.h>
#include <Wt/WString.h>
#include <Wt/WLink.h>
#include <Wt/WAnchor.h>
#include <Wt/WMessageBox.h>
#include <Wt/WLocalDateTime.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

ProjectSummaryWidget::ProjectSummaryWidget(Session &session, const std::string &basePath, dbo::ptr<Project> project, const std::string uploadPath)
    : WContainerWidget(),
      session_(session),
      basePath_(basePath),
      project_(project),
      uploadPath_(uploadPath)
{
    resize(800, 150);
    setStyleClass("project-box");
    setPadding(30);

    hBox_ = setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());
    projectImage_ = hBox_->addWidget(Wt::cpp14::make_unique<Wt::WImage>());
    projectImage_->setMaximumSize(150, 150);

    vBox_ = hBox_->addLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>(), 1);
    projectName_ = vBox_->addWidget(Wt::cpp14::make_unique<Wt::WAnchor>());
    projectDesc_ = vBox_->addWidget(Wt::cpp14::make_unique<Wt::WText>(), 1);
    projectDesc_->setPadding(20, Wt::Side::Right);

    auto iconBox = vBox_->addLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    Wt::WLocalDateTime ldateTime = project_->getCreationDate().toLocalTime();

    if (ldateTime.isValid()) {
        ;//std::string ldatetime = ldateTime.toString().toUTF8();
    }
    
    Wt::WString datetime = "<small><i>" 
                            + project_->getCreationDate().toString("dddd, MMMM d, yyyy @ HH:mm") + 
                            "</i></small>";

    iconBox->addWidget(Wt::cpp14::make_unique<Wt::WText>(datetime));

    auto shareIcon = Wt::cpp14::make_unique<Wt::WImage>("icons/share.svg");
    shareIcon->setMaximumSize(20, 20);
    shareIcon_ = iconBox->addWidget(std::move(shareIcon));
    shareIcon_->setStyleClass("project-icons");

    auto deleteIcon = Wt::cpp14::make_unique<Wt::WImage>("icons/delete.svg");
    deleteIcon->setMaximumSize(20, 20);
    deleteIcon_ = iconBox->addWidget(std::move(deleteIcon));
    deleteIcon_->setStyleClass("project-icons");
    deleteIcon_->clicked().connect([=] {
        deleteDialog();
    });

    buildProject();
    render();
}

void ProjectSummaryWidget::buildProject()
{
    bool newProject = project_.id() == -1;

    dbo::Transaction t(session_);
    
    if (newProject) {
        session_.add(project_);
    }
    
    session_.flush();
    
    t.commit();
}

void ProjectSummaryWidget::render()
{
    projectName_->setLink(Wt::WLink(Wt::LinkType::InternalPath, (basePath_ + "viewer/" + project_->titleToUrl())));
    projectName_->setText("<h4>" + project_->getTitle() + "</h4>");
    projectDesc_->setText(project_->getDescription());
    projectImage_->setImageLink("icons/model.png");
}

void ProjectSummaryWidget::rm()
{
    dbo::Transaction t(session_);

    project_.remove();
    t.commit();

    std::string userFolder = uploadPath_ + "/" + session_.user()->name.toUTF8();
    std::string path{ userFolder + "/" + project_->titleToUrl() };

    if(pathExists(userFolder) && pathExists(path))
        deleteFolder(path);

    this->removeFromParent();
}

void ProjectSummaryWidget::deleteDialog()
{
    Wt::StandardButton answer = Wt::WMessageBox::show(
                                    "FxTract", "<p>Delete this project, \"" + project_->getTitle() + "\" ?</p>",
                                    Wt::StandardButton::Ok | Wt::StandardButton::Cancel
                                );
    if (answer == Wt::StandardButton::Ok) {
        rm();
    }
}

ProjectSummaryWidget::~ProjectSummaryWidget() {}