#ifndef USER_H_
#define USER_H_

#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include "Project.h"
#include "Token.h"

class ModelFile;
class User;
class Project;
class Token;

namespace dbo = Wt::Dbo;

using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;
typedef dbo::collection< dbo::ptr<Project> > Projects;
typedef dbo::collection< dbo::ptr<ModelFile> > ModelFiles;
typedef dbo::collection< dbo::ptr<Token> > Tokens;

class User {
public:
    /* You probably want to add other user information here */
    Wt::WString name;
    std::string password;
    std::string passwordMethod;
    std::string passwordSalt;
    int failedLoginAttempts;
    Wt::WDateTime lastLoginAttempt;

    std::string oAuthId;
    std::string oAuthProvider;

    Tokens authTokens;
    Projects projects;
    ModelFiles modelfiles;

    Projects allProjects() const;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, name, "name");
        dbo::field(a, password, "password");
        dbo::field(a, passwordMethod, "password_method");
        dbo::field(a, passwordSalt, "password_salt");
        dbo::field(a, failedLoginAttempts, "failed_login_attempts");
        dbo::field(a, lastLoginAttempt, "last_login_attempt");
        dbo::field(a, oAuthId, "oauth_id");
        dbo::field(a, oAuthProvider, "oauth_provider");

        dbo::hasMany(a, projects, dbo::ManyToOne, "author");
        dbo::hasMany(a, modelfiles, dbo::ManyToOne, "author");
        dbo::hasMany(a, authTokens, dbo::ManyToOne, "user");
    }

    static dbo::dbo_traits<User>::IdType stringToId(const std::string &s);
};

DBO_EXTERN_TEMPLATES(User)

#endif // USER_H_