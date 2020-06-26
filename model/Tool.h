#ifndef TOOL_H_
#define TOOL_H_

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>

namespace dbo = Wt::Dbo;

class Tool {
public:
  void addTool(std::string tool_id,
  std::string tool_name,
  double tool_angle,
  double tool_length,
  double tool_min_radius,
  double tool_max_radius);

  std::string tool_id;
  std::string tool_name;
  double tool_angle;
  double tool_length;
  double tool_min_radius;
  double tool_max_radius;

  template<class Action>
  void persist(Action& a) {
    dbo::id(a, tool_id, "tool_id", 20);
    dbo::field(a, tool_name, "tool_name");
    dbo::field(a, tool_angle, "tool_angle");
    dbo::field(a, tool_length, "tool_length");
    dbo::field(a, tool_min_radius, "tool_min_radius");
    dbo::field(a, tool_max_radius, "tool_max_radius");
  }
};

namespace Wt {
  namespace Dbo {
    template<>
    struct dbo_traits<Tool> : public dbo_default_traits {
      typedef std::string IdType;
      static IdType invalidId() {
        return std::string();
      }
      static const char *surrogateIdField() { return 0; }
    };
  }
}

DBO_EXTERN_TEMPLATES(Tool)

#endif // TOOL_H_
