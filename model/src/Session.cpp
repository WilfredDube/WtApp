#include "../include/Session.h"
#include "../include/BendFeature.h"
#include "../include/ModelFile.h"
#include "../include/Project.h"
#include "../include/Token.h"
#include "../include/ProcessPlan.h"
#include "../include/BendSequence.h"

#include "../../dbdao/include/UserDao.h"
#include "../../dbdao/include/ToolDao.h"
#include "../../dbdao/include/MaterialDao.h"

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
using namespace Fxt::Dao;

namespace 
{
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
  mapClass<BendSequence>("bend_sequence");
  mapClass<ProcessPlan>("process_plan");
  mapClass<Material>("material");
  mapClass<Tool>("tools");

  // mapClass<AuthInfo>("auth_info");
  // mapClass<AuthInfo::AuthIdentityType>("auth_identity");
  // mapClass<AuthInfo::AuthTokenType>("auth_token");

  try {
    dbo::Transaction transaction(*this);

    createTables();

    UserDao userDao{*this};
    dbo::ptr<User> guest = userDao.insert("guest", "guest");

    Auth::User guestUser = users_.findWithIdentity(Wt::Auth::Identity::LoginName, guest->name);
    // myPasswordService.updatePassword(guestUser, "guest");

    fillTools();
    fillMaterial();

    transaction.commit();

  } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      std::cerr << "Using existing database";
  }
}

void Session::fillMaterial()
{
  MaterialDao materialDao{*this};

  materialDao.insert("Aluminium", 310, 0.43);
  materialDao.insert("Mild Steel", 440, 0.45);
  materialDao.insert("Stainless Steel", 505, 0.45);
}

void Session::fillTools()
{
  ToolDao toolDao{*this};

  toolDao.insert("AC26", "ACUTE", 26, 1000, 0.2, 50);
  toolDao.insert("AC30", "ACUTE", 30, 1000, 0.2, 50);
  toolDao.insert("AC35", "ACUTE", 35, 1000, 0.2, 50);
  toolDao.insert("AC45", "ACUTE", 45, 1000, 0.2, 50);
  toolDao.insert("AC60", "ACUTE", 60, 1000, 0.2, 50);
  toolDao.insert("AC88", "GOOSENECK", 88, 1000, 0.2, 50);
  toolDao.insert("AC90", "GOOSENECK", 90, 1000, 0.2, 50);
  toolDao.insert("VT15", "VTOOL", 15, 1000, 0.2, 50);
  toolDao.insert("VT20", "VTOOL", 20, 1000, 0.2, 50);
  toolDao.insert("VT30", "VTOOL", 30, 1000, 0.2, 50);
  toolDao.insert("VT35", "VTOOL", 35, 1000, 0.2, 50);
  toolDao.insert("VT45", "VTOOL", 45, 1000, 0.2, 50);
  toolDao.insert("VT60", "VTOOL", 60, 1000, 0.2, 50);
  toolDao.insert("VT88", "VTOOL", 88, 1000, 0.2, 50);
  toolDao.insert("VT90", "VTOOL", 90, 1000, 0.2, 50);
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