#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/SqlConnectionPool.h>

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>

#include "view/FXtractUI.h"
#include "model/Session.h"

// export LD_LIBRARY_PATH=/usr/local/lib64/

class FXtractApp : public Wt::WApplication
{
public:
  FXtractApp(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool& connectionPool)
    : Wt::WApplication(env)
  {
    root()->addWidget(Wt::cpp14::make_unique<FXtractUI>("/", connectionPool));
  }
};

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env,
                                Wt::Dbo::SqlConnectionPool *connectionPool)
{
  return Wt::cpp14::make_unique<FXtractApp>(env,  *connectionPool);
}

int main(int argc, char ** argv)
{
  try {
    Wt::WServer server{argc, argv, WTHTTP_CONFIGURATION};

    Session::configureAuth();

    auto fxtractDb = Session::createConnectionPool(server.appRoot() + "fxtract.db");

    server.addEntryPoint(Wt::EntryPointType::Application, 
                         std::bind(&createApplication, std::placeholders::_1, fxtractDb.get()), 
                         "/app");

    server.run();
  } catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (Wt::Dbo::Exception &e) {
    std::cerr << "Dbo exception: " << e.what() << std::endl;
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
  }
}