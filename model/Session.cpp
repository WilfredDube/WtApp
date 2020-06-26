#include "Session.h"
#include "BendFeature.h"
#include "ModelFile.h"
#include "Project.h"
#include "Token.h"
#include "User.h"
#include "Tool.h"
#include "ProcessPlan.h"
#include "MachineParam.h"
#include "BendSequence.h"

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/Identity.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Auth/GoogleService.h>

#include <Wt/Dbo/FixedSqlConnectionPool.h>
// #include <Wt/Dbo/SqlConnectionPool.h>

// #include <Wt/Dbo/backend/Sqlite3.h>
// #include <Wt/WApplication.h>

using namespace Wt;

namespace {

    class FXtractOAuth : public std::vector<const Wt::Auth::OAuthService *>
    {
      public:
        ~FXtractOAuth()
        {
          for (size_t i = 0; i < size(); i++)
          {
            delete (*this)[i];
          }
          
        }
    };

    Auth::AuthService myAuthService;
    Auth::PasswordService myPasswordService(myAuthService);
    FXtractOAuth myOAuthServices;
}

void Session::configureAuth()
{
    myAuthService.setAuthTokensEnabled(true, "fxtractcookie");
    // myAuthService.setEmailVerificationEnabled(true);

    std::unique_ptr<Auth::PasswordVerifier> verifier
        = cpp14::make_unique<Auth::PasswordVerifier>();

    verifier->addHashFunction(cpp14::make_unique<Auth::BCryptHashFunction>(7));
    myPasswordService.setVerifier(std::move(verifier));
    myPasswordService.setAttemptThrottlingEnabled(true);

    myPasswordService.setStrengthValidator(
        cpp14::make_unique<Auth::PasswordStrengthValidator>()
    );

    if (Auth::GoogleService::configured())
    {
        myOAuthServices.push_back(new Wt::Auth::GoogleService(myAuthService));
    }

    for (size_t i = 0; i < myOAuthServices.size(); ++i)
    {
        myOAuthServices[i]->generateRedirectEndpoint();
    }   
}

std::unique_ptr<dbo::SqlConnectionPool> Session::createConnectionPool(const std::string& sqliteDb)
{
  auto connection = Wt::cpp14::make_unique<dbo::backend::Sqlite3>(sqliteDb);

  connection->setProperty("show-queries", "true");
  connection->setDateTimeStorage(Wt::Dbo::SqlDateTimeType::DateTime, Wt::Dbo::backend::DateTimeStorage::PseudoISO8601AsText);

  return Wt::cpp14::make_unique<dbo::FixedSqlConnectionPool>(std::move(connection), 10);
}

Session::Session(dbo::SqlConnectionPool& connectionPool)
  : connectionPool_(connectionPool),
  users_(*this)
{
  setConnectionPool(connectionPool_);

  mapClass<BendFeature>("bend_feature");
  mapClass<ModelFile>("model_file");
  mapClass<Project>("project");
  mapClass<Token>("token");
  mapClass<User>("user");

  mapClass<MachineParam>("machine_param");
  mapClass<BendSequence>("bend_sequence");
  mapClass<ProcessPlan>("process_plan");
  

  mapClass<Tool>("tools");
  
//   #include "ProcessPlan.h"
// #include "MachineParam.h"
// #include "BendSequence.h"
  // mapClass<AuthInfo>("auth_info");
  // mapClass<AuthInfo::AuthIdentityType>("auth_identity");
  // mapClass<AuthInfo::AuthTokenType>("auth_token");

  try {
    dbo::Transaction transaction(*this);  
    createTables();

    dbo::ptr<User> guest = add(Wt::cpp14::make_unique<User>());
    User *u = guest.modify();
    u->name = "guest";
    u->password = "guest";

    Auth::User guestUser = users_.findWithIdentity(Wt::Auth::Identity::LoginName, u->name);
    myPasswordService.updatePassword(guestUser, "guest");

    fillTools();

    std::cerr << "Created database." << std::endl;

    transaction.commit();

  } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      std::cerr << "Using existing database";
  }
}

void Session::fillTools()
{
  dbo::ptr<Tool> MFA(Wt::cpp14::make_unique<Tool>());
  Tool *a = MFA.modify();
  a->addTool("AC26", "ACUTE", 26, 1000, 0.2, 50);
  add(MFA);

  dbo::ptr<Tool> MFB(Wt::cpp14::make_unique<Tool>());
  Tool *b = MFB.modify();
  b->addTool("AC30", "ACUTE", 30, 1000, 0.2, 50);
  add(MFB);

  dbo::ptr<Tool> MFC(Wt::cpp14::make_unique<Tool>());
  Tool *c = MFC.modify();
  c->addTool("AC35", "ACUTE", 35, 1000, 0.2, 50);
  add(MFC);

  dbo::ptr<Tool> MFD(Wt::cpp14::make_unique<Tool>());
  Tool *d = MFD.modify();
  d->addTool("AC45", "ACUTE", 45, 1000, 0.2, 50);
  add(MFD);

  dbo::ptr<Tool> MFE(Wt::cpp14::make_unique<Tool>());
  Tool *e = MFE.modify();
  e->addTool("AC60", "ACUTE", 60, 1000, 0.2, 50);
  add(MFE);

  dbo::ptr<Tool> MFF(Wt::cpp14::make_unique<Tool>());
  Tool *f = MFF.modify();
  f->addTool("AC88", "GOOSENECK", 88, 1000, 0.2, 50);
  add(MFF);

  dbo::ptr<Tool> MFG(Wt::cpp14::make_unique<Tool>());
  Tool *g = MFG.modify();
  g->addTool("AC90", "GOOSENECK", 90, 1000, 0.2, 50);
  add(MFG);

  dbo::ptr<Tool> MFH(Wt::cpp14::make_unique<Tool>());
  Tool *h = MFH.modify();
  h->addTool("VT15", "VTOOL", 15, 1000, 0.2, 50);
  add(MFH);

  dbo::ptr<Tool> MFI(Wt::cpp14::make_unique<Tool>());
  Tool *i = MFI.modify();
  i->addTool("VT20", "VTOOL", 20, 1000, 0.2, 50);
  add(MFI);

  dbo::ptr<Tool> MFJ(Wt::cpp14::make_unique<Tool>());
  Tool *j = MFJ.modify();
  j->addTool("VT30", "VTOOL", 30, 1000, 0.2, 50);
  add(MFJ);

  dbo::ptr<Tool> MFK(Wt::cpp14::make_unique<Tool>());
  Tool *k = MFK.modify();
  k->addTool("VT35", "VTOOL", 35, 1000, 0.2, 50);
  add(MFK);

  dbo::ptr<Tool> MFL(Wt::cpp14::make_unique<Tool>());
  Tool *l = MFL.modify();
  l->addTool("VT45", "VTOOL", 45, 1000, 0.2, 50);
  add(MFL);

  dbo::ptr<Tool> MFM(Wt::cpp14::make_unique<Tool>());
  Tool *m = MFM.modify();
  m->addTool("VT60", "VTOOL", 60, 1000, 0.2, 50);
  add(MFM);

  dbo::ptr<Tool> MFN(Wt::cpp14::make_unique<Tool>());
  Tool *n = MFN.modify();
  n->addTool("VT88", "VTOOL", 88, 1000, 0.2, 50);
  add(MFN);

  dbo::ptr<Tool> MFO(Wt::cpp14::make_unique<Tool>());
  Tool *o = MFO.modify();
  o->addTool("VT90", "VTOOL", 90, 1000, 0.2, 50);
  add(MFO);
}

dbo::ptr<User> Session::user() const
{
  if (login_.loggedIn())  
    return users_.find(login_.user());  
  else
    return dbo::ptr<User>();
}

FxtUserDatabase& Session::users()
{
  return users_;
}

// const Auth::AuthService& Session::auth()
// {
//   return myAuthService;
// }

const Auth::PasswordService *Session::passwordAuth()
{
  return &myPasswordService;
}

const std::vector<const Auth::OAuthService *>& Session::oAuth()
{
  // std::vector<const Auth::OAuthService *> result;
  // for (auto &auth : myOAuthServices) {
  //   result.push_back(auth.get());
  // }
  return myOAuthServices;//(result);
}