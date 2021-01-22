#pragma once

#include <Wt/WDate.h>
#include <Wt/WGlobal.h>
#include <Wt/WString.h>

#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <string>

namespace dbo = Wt::Dbo;

class Material 
{
  std::string material_name;
  double tensile_strength;
  double k_factor;

  template<class Action>
  void persist(Action& a) {
    dbo::id(a, material_name, "material_name", 20);
    dbo::field(a, tensile_strength, "tensile_strength");
    dbo::field(a, k_factor, "k_factor");
  }
};

namespace Wt {
  namespace Dbo {
    template<>
    struct dbo_traits<Material> : public dbo_default_traits {
      typedef std::string IdType;
      static IdType invalidId() {
        return std::string();
      }
      static const char *surrogateIdField() { return 0; }
    };
  }
}

DBO_EXTERN_TEMPLATES(Material)