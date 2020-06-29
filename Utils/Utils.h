#ifndef UTILS_H_
#define UTILS_H_

#include <string>

static const double Aluminium = 310; // 0.43
static const double MildSteel = 440; // 0.45
static const double StainlessSteel = 505; // 0.45

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