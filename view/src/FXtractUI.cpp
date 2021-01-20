#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/Auth/AuthModel.h>
#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/RegistrationModel.h>
#include <Wt/WApplication.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WEnvironment.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WMenu.h>
#include <memory>
#include <vector>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

#include "../include/FXtractUI.h"
#include "../include/NavBarWidget.h"
#include "../include/ModelViewerWidget.h"
#include "../include/ProjectListWidget.h"

#include "../../Utils/Utils.h"
#include "../../model/include/Session.h"

using namespace Wt;
namespace fs = std::filesystem;

namespace {
  const std::string UPLOAD_FOLDER = "uploads";
  const std::string TEMP_FOLDER = "/temp";
}

FXtractUI::FXtractUI(const std::string& basePath, Wt::Dbo::SqlConnectionPool& connectionPool)
    : WContainerWidget(),
    session_(connectionPool),
    basePath_(basePath),
    projectListContainer_(nullptr),
    modelViewer_(nullptr),
    userChanged_()
{
  Wt::WApplication *app = Wt::WApplication::instance();

  if (!fs::exists(UPLOAD_FOLDER))
  {
    fs::create_directories(UPLOAD_FOLDER);
    std::cout << "\n***Done : Upload folder created..." << std::endl;
  }  

  app->setTitle("FxTract");

  auto theme = std::make_shared<Wt::WBootstrapTheme>();
  theme->setVersion(Wt::BootstrapVersion::v3);
  app->setTheme(theme);
  app->useStyleSheet("style.css");

  app->messageResourceBundle().use(app->appRoot() + "templates");

  session_.login().changed().connect(this, &FXtractUI::onAuthEvent);
  
  auto authModel
      = cpp14::make_unique<Auth::AuthModel>(Session::passwordAuth()->baseAuth(), session_.users());
  authModel->addPasswordAuth(session_.passwordAuth());
  authModel->addOAuth(session_.oAuth());

  std::unique_ptr<Auth::AuthWidget> authWidget
      = cpp14::make_unique<Auth::AuthWidget>(session_.login());
  auto authWidgetPtr = authWidget.get();
  authWidget->setModel(std::move(authModel));
  authWidget->setRegistrationEnabled(true);
  // authWidget->setInternalBasePath(basePath + "login");

  auto navBarWidget = cpp14::make_unique<NavBarWidget>("FxTract", "#", basePath_);
  navBarWidget->addStyleClass("nav navbar", true);
  navBarWidget_ = navBarWidget.get();

  addWidget(std::move(navBarWidget));
  navBarWidget_->hide();

  auto title = cpp14::make_unique<WText>("<h1>FxTract</h1>");
  title_ = title.get();
  addWidget(std::move(title));

  addWidget(std::move(authWidget));

  mainStack_ = addNew<Wt::WStackedWidget>();
  mainStack_->addStyleClass("contents");

  WApplication::instance()->internalPathChanged()
  .connect(this, &FXtractUI::handleInternalPathChange);

  authWidgetPtr->processEnvironment();
  onAuthEvent();
}

void FXtractUI::onAuthEvent()
{
  if (session_.login().loggedIn()) {   
    std::string userFolder = UPLOAD_FOLDER + "/" + session_.user()->name.toUTF8(); 
    if (!fs::exists(userFolder))
    {
      fs::create_directories(userFolder);
      std::cout << "\n***Done : User folder created >> " + userFolder << std::endl;
    }

    if (!fs::exists(userFolder + TEMP_FOLDER))
    {
      fs::create_directories(userFolder + TEMP_FOLDER);
      std::cout << "\n***Done : user temp folder created..." << std::endl;
    }
      
    loggedIn();    
  } else {
    loggedOut();
  }
}

void FXtractUI::setInternalBasePath(const std::string& basePath)
{
  basePath_ = basePath;
  refresh();
}

void FXtractUI::refresh()
{
  handleInternalPathChange(WApplication::instance()->internalPath());
}

void FXtractUI::loggedIn()
{
  Wt::WApplication::instance()->changeSessionId();

  refresh();

  title_->hide();
  navBarWidget_->show();

  displayProjects();
}

void FXtractUI::displayProjects()
{
  if(!session_.user())
    return;

  dbo::Transaction t(session_);

  if (projectListContainer_)
  {
    projectListContainer_->refreshProjects();
    projectListContainer_->showProjects(session_.user()->allProjects());
  }
  
  t.commit();
}

void FXtractUI::loggedOut()
{
  refresh();
  navBarWidget_->hide();
  title_->show();
  mainStack_->clear();
  modelViewer_ = 0;
  projectListContainer_ = 0;
}

void FXtractUI::handleInternalPathChange(const std::string&)
{
  Wt::WApplication *app = Wt::WApplication::instance();

  if (session_.login().loggedIn()) {
    if (app->internalPathMatches(basePath_))
    {
      dbo::Transaction t(session_);

      std::string path = app->internalPathNextPart(basePath_);

      projectListContainer_ = nullptr;
      modelViewer_ = nullptr;

      if (path.empty())
      {
        showProjects();
      } else if (path == "viewer") {
        std::string projectname = app->internalPathNextPart(basePath_ + path + '/');
        dbo::ptr<Project> project = session_.find<Project>()
            .where("LOWER(project_title) LIKE LOWER(?)").bind(urltoTitle(projectname))
            .where("author_id = ?").bind(session_.user().id());

        if (!project)
        {
          app->setInternalPath(basePath_);
        }

        if (!projectname.empty() && project)
        {
          showModelViewer(project);
        }  
      } else {
        WApplication::instance()->setInternalPath("/",  true); 
      }   
      
      t.commit();
    }
  } 
  
  // if (session_.login().loggedIn()) {
  //   if (internalPath == "/")
  //       showProjects();
  //   else if (internalPath == "viewer")
  //       showModelViewer();
  //   else{
  //     navBarWidget_->hide();
  //     title_->show();
  //     WApplication::instance()->setInternalPath("/",  true);      
  //   }
  // }
}

void FXtractUI::showProjects()
{
  if (!projectListContainer_)
  {
    projectListContainer_ = mainStack_->addWidget(Wt::cpp14::make_unique<ProjectListWidget>(basePath_, session_, UPLOAD_FOLDER));
  }
  
  displayProjects();
  mainStack_->setCurrentWidget(projectListContainer_);
}

void FXtractUI::showModelViewer(dbo::ptr<Project> project)
{
  if(!modelViewer_){
    modelViewer_ = mainStack_->addWidget(cpp14::make_unique<ModelViewerWidget>(session_, project, UPLOAD_FOLDER));
  }

  mainStack_->setCurrentWidget(modelViewer_);
}

void FXtractUI::logout() { session_.login().logout(); }