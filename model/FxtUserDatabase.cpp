/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Identity.h>

#include "FxtUserDatabase.h"
#include "User.h"
#include "Token.h"

#include <string>

class InvalidUser : public std::runtime_error
{
public:
  InvalidUser(const std::string& id)
    : std::runtime_error("Invalid user: " + id)
  { }
};

class TransactionImpl : public Wt::Auth::AbstractUserDatabase::Transaction,
			public dbo::Transaction
{
public:
  TransactionImpl(dbo::Session& session)
    : dbo::Transaction(session)
  { }

  virtual ~TransactionImpl() noexcept(false)
  { }

  virtual void commit()
  {
    dbo::Transaction::commit();
  }

  virtual void rollback()
  {
    dbo::Transaction::rollback();
  }
};

FxtUserDatabase::FxtUserDatabase(dbo::Session& session)
  : session_(session)
{ }

Wt::Auth::AbstractUserDatabase::Transaction *FxtUserDatabase::startTransaction()
{
  return new TransactionImpl(session_);
}

dbo::ptr<User> FxtUserDatabase::find(const Wt::Auth::User& user) const
{
  getUser(user.id());

  return user_;
}

Wt::Auth::User FxtUserDatabase::find(const dbo::ptr<User> user) const
{
  user_ = user;

  return Wt::Auth::User(std::to_string(user_.id()), *this);
}

Wt::Auth::User FxtUserDatabase::findWithId(const std::string& id) const
{
  getUser(id);

  if (user_)
    return Wt::Auth::User(id, *this);
  else
    return Wt::Auth::User();
}

Wt::Auth::PasswordHash FxtUserDatabase::password(const Wt::Auth::User& user) const
{
  WithUser find(*this, user);

  return Wt::Auth::PasswordHash(user_->passwordMethod, user_->passwordSalt,
			    user_->password);
}

void FxtUserDatabase::setPassword(const Wt::Auth::User& user,
                                   const Wt::Auth::PasswordHash& password)
{
  WithUser find(*this, user);

  user_.modify()->password = password.value();
  user_.modify()->passwordMethod = password.function();
  user_.modify()->passwordSalt = password.salt();
}

Wt::Auth::User::Status FxtUserDatabase::status(const Wt::Auth::User& user) const
{
  return Wt::Auth::AccountStatus::Normal;
}

void FxtUserDatabase::setStatus(const Wt::Auth::User& user,
                                 Wt::Auth::User::Status status)
{
  throw std::runtime_error("Changing status is not supported.");
}

void FxtUserDatabase::addIdentity(const Wt::Auth::User& user,
				   const std::string& provider,
                                   const Wt::WString& identity)
{
  WithUser find(*this, user);

  if (provider == Wt::Auth::Identity::LoginName)
    user_.modify()->name = identity;
  else {
    user_.modify()->oAuthProvider = provider;
    user_.modify()->oAuthId = identity.toUTF8();
  }
}

Wt::WString FxtUserDatabase::identity(const Wt::Auth::User& user,
				       const std::string& provider) const
{
  WithUser find(*this, user);

  if (provider == Wt::Auth::Identity::LoginName)
    return user_->name;
  else if (provider == user_->oAuthProvider)
    return Wt::WString(user_->oAuthId);
  else
    return Wt::WString::Empty;
}

void FxtUserDatabase::removeIdentity(const Wt::Auth::User& user,
				      const std::string& provider)
{
  WithUser find(*this, user);

  if (provider == Wt::Auth::Identity::LoginName)
    user_.modify()->name = "";
  else if (provider == user_->oAuthProvider) {
    user_.modify()->oAuthProvider = std::string();
    user_.modify()->oAuthId = std::string();
  }
}

Wt::Auth::User FxtUserDatabase::findWithIdentity(const std::string& provider,
                                              const Wt::WString& identity) const
{
  dbo::Transaction t(session_);
  if (provider == Wt::Auth::Identity::LoginName) {
    if (!user_ || user_->name != identity)
      user_ = session_.find<User>().where("name = ?").bind(identity);
  } else
    user_ = session_.find<User>()
      .where("oauth_id = ?").bind(identity.toUTF8())
      .where("oauth_provider = ?").bind(provider);
  t.commit();

  if (user_)
    return Wt::Auth::User(std::to_string(user_.id()), *this);
  else
    return Wt::Auth::User();
}

Wt::Auth::User FxtUserDatabase::registerNew()
{
  user_ = session_.add(Wt::cpp14::make_unique<User>());
  user_.flush();
  return Wt::Auth::User(std::to_string(user_.id()), *this);
}

void FxtUserDatabase::addAuthToken(const Wt::Auth::User& user,
                                    const Wt::Auth::Token& token)
{
  WithUser find(*this, user);

  /*
   * This should be statistically very unlikely but also a big
   * security problem if we do not detect it ...
   */
  if (session_.find<Token>().where("value = ?")
      .bind(token.hash()).resultList().size() > 0)
    throw std::runtime_error("Token hash collision");

  /*
   * Prevent a user from piling up the database with tokens
   */
  if (user_->authTokens.size() > 50)
    return;

  user_.modify()->authTokens.insert
    (dbo::ptr<Token>(Wt::cpp14::make_unique<Token>(token.hash(), token.expirationTime())));
}

int FxtUserDatabase::updateAuthToken(const Wt::Auth::User& user,
				      const std::string& hash,
				      const std::string& newHash)
{
  WithUser find(*this, user);

  for (Tokens::const_iterator i = user_->authTokens.begin();
       i != user_->authTokens.end(); ++i) {
    if ((*i)->value == hash) {
      dbo::ptr<Token> p = *i;
      p.modify()->value = newHash;
      return std::max(Wt::WDateTime::currentDateTime().secsTo(p->expires), 0);
    }
  }

  return 0;
}

void FxtUserDatabase::removeAuthToken(const Wt::Auth::User& user,
				       const std::string& hash)
{
  WithUser find(*this, user);

  for (Tokens::const_iterator i = user_->authTokens.begin();
       i != user_->authTokens.end(); ++i) {
    if ((*i)->value == hash) {
      dbo::ptr<Token> p = *i;
      p.remove();
      break;
    }
  }
}

Wt::Auth::User FxtUserDatabase::findWithAuthToken(const std::string& hash) const
{
  dbo::Transaction t(session_);
  user_ = session_.query< dbo::ptr<User> >
    ("select u from \"user\" u join token t on u.id = t.user_id")
    .where("t.value = ?").bind(hash)
    .where("t.expires > ?").bind(Wt::WDateTime::currentDateTime());
  t.commit();

  if (user_)
    return Wt::Auth::User(std::to_string(user_.id()), *this);
  else
    return Wt::Auth::User();
}

int FxtUserDatabase::failedLoginAttempts(const Wt::Auth::User& user) const
{
  WithUser find(*this, user);

  return user_->failedLoginAttempts;
}

void FxtUserDatabase::setFailedLoginAttempts(const Wt::Auth::User& user,
					      int count)
{
  WithUser find(*this, user);

  user_.modify()->failedLoginAttempts = count;
}

Wt::WDateTime FxtUserDatabase::lastLoginAttempt(const Wt::Auth::User& user) const
{
  WithUser find(*this, user);

  return user_->lastLoginAttempt;
}

void FxtUserDatabase::setLastLoginAttempt(const Wt::Auth::User& user,
                                           const Wt::WDateTime& t)
{ 
  WithUser find(*this, user);

  user_.modify()->lastLoginAttempt = t;
}

void FxtUserDatabase::getUser(const std::string& id) const
{
  if (!user_ || std::to_string(user_.id()) != id) {
    dbo::Transaction t(session_);
    user_ = session_.find<User>()
      .where("id = ?").bind(User::stringToId(id));
    t.commit();
  }
}

FxtUserDatabase::WithUser::WithUser(const FxtUserDatabase& self,
                                     const Wt::Auth::User& user)
  : transaction(self.session_)
{
  self.getUser(user.id());

  if (!self.user_)
    throw InvalidUser(user.id());
}

FxtUserDatabase::WithUser::~WithUser()
{
  transaction.commit();
}
