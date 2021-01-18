#pragma once

#include "../../model/include/User.h"
#include "../../model/include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<User> > Users;

class UserDao
{
public:

    UserDao(Session& session) : session { session } {}

    Wt::Dbo::ptr<User> insert(std::string name, std::string password, std::string email = "");

    Wt::Dbo::ptr<User> get(std::string name);

    Users getAll();

    bool deleteProject(Wt::Dbo::ptr<User>& user);

private:
    Session& session;
};