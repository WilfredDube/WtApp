#ifndef UTILS_H_
#define UTILS_H_

#include <string>

static const double Aluminium = 10;
static const double MildSteel = 20;
static const double StainlessSteel = 40;

double tensileStrength(const std::string material)
{
  if (material == "Aluminium")
  {
    return Aluminium;
  } else if (material == "Mild Steel")
    return MildSteel;

  return StainlessSteel;  
}

std::string urltoTitle(std::string url)
{
 std::string result = url;
  for (unsigned i = 0; i < result.length(); ++i) {
    if (result[i] == '_')
      result[i] = ' ';
    else
      result[i] = tolower(result[i]);
  }
  
  return result;
}
#endif