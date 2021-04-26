#include "../include/UserDao.h"

#include <memory>

namespace dbo = Wt::Dbo;

using namespace Fxt::Dao;

Wt::Dbo::ptr<User> UserDao::insert(std::string name, std::string password, std::string email)
{
    auto ret = Wt::Dbo::ptr<User>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<User> user(Wt::cpp14::make_unique<User>());
        user->name = name;
        user->password = password;
        // user->email = email;
        
        ret = session.add(std::move(user));
    }
    
    return ret;
}

dbo::ptr<User> UserDao::get(std::string name)
{
    dbo::Transaction transaction(session);
    
    dbo::ptr<User> user = session.query<dbo::ptr<User>>("select u from user u")
                                        .where("name = ?").bind(name);

    return user;
}

Users UserDao::getAll()
{
    return session.find<User>();
}

bool UserDao::deleteProject(dbo::ptr<User>& user)
{
    dbo::Transaction transaction(session);

    user.remove();

    return true;
}