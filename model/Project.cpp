#include "Project.h"
#include "ModelFile.h"
#include "User.h"

#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

DBO_INSTANTIATE_TEMPLATES(Project)

size_t Project::modelCount() const
{
  return modelFiles.size();   
}

std::string Project::titleToUrl() const
{
  std::string result = title.narrow();
  for (unsigned i = 0; i < result.length(); ++i) {
    if (!isalnum(result[i]))
      result[i] = '_';
    else {
      result[i] = tolower(result[i]);
    }
  }
  
  return result;
}