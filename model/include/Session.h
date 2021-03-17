#pragma once

#include <Wt/Auth/Login.h>
#include <Wt/Auth/Dbo/UserDatabase.h>

#include <Wt/Dbo/Session.h>
#include <Wt/WSignal.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/backend/Sqlite3.h>

#include "FxtUserDatabase.h"

namespace dbo = Wt::Dbo;

class BendFeature;
class User;
class Project;
class ModelFile;
class BendSequence;
class ProcessPlan;

class Session : public dbo::Session
{
public:
    static void configureAuth();

    Session(dbo::SqlConnectionPool& connectionPool);

    dbo::ptr<User> user() const;

    Wt::Signal<dbo::ptr<ModelFile> >& modelFileChanged(){ return modelFileUploaded_; }
    Wt::Signal<dbo::ptr<ModelFile> >& modelFileClicked(){ return modelFileClicked_; }
    Wt::Signal<dbo::ptr<ModelFile> >& modelFileDeleted(){ return modelFileDelete_; }
    Wt::Signal<dbo::ptr<ModelFile> >& modelFeaturesExtracted() { return modelFeaturesExtracted_; }
    Wt::Signal<dbo::ptr<ModelFile> >& modelFeaturesTableUpdate() { return modelFeaturesTableUpdate_; }

    FxtUserDatabase& users();
    Wt::Auth::Login& login() { return login_; }

    static const Wt::Auth::PasswordService *passwordAuth();
    static const std::vector<const Wt::Auth::OAuthService *>& oAuth();

    static std::unique_ptr<dbo::SqlConnectionPool> createConnectionPool(const std::string& sqlite3);

private:
    dbo::SqlConnectionPool& connectionPool_;
    FxtUserDatabase users_;
    Wt::Auth::Login login_;

    Wt::Signal< dbo::ptr<ModelFile> > modelFileUploaded_;
    Wt::Signal< dbo::ptr<ModelFile> > modelFileClicked_;
    Wt::Signal< dbo::ptr<ModelFile> > modelFileDelete_;
    Wt::Signal< dbo::ptr<ModelFile> > modelFeaturesExtracted_;
    Wt::Signal< dbo::ptr<ModelFile> > modelFeaturesTableUpdate_;

    void fillTools();
    void fillMaterial();
};