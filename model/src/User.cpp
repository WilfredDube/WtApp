#include "../include/User.h"

#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

DBO_INSTANTIATE_TEMPLATES(User)

Projects User::allProjects() const 
{
    return projects.find().orderBy("project_creation_date desc");
}

dbo::dbo_traits<User>::IdType User::stringToId(const std::string &s)
{
  std::size_t pos = std::string::npos;
  auto result = std::stoll(s, &pos);
  if (pos != s.size())
    return dbo::dbo_traits<User>::invalidId();
  else
    return result;
}