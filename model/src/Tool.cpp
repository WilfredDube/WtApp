#include "Tool.h"

#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

void Tool::addTool(std::string toolId, std::string toolName, double toolAngle,
  double toolLength, double toolMinRadius, double toolMaxRadius)
{
  tool_id = (toolId);
  tool_name = (toolName);
  tool_angle = (toolAngle);
  tool_length = (toolLength);
  tool_min_radius = (toolMinRadius);
  tool_max_radius = (toolMaxRadius);
}

DBO_INSTANTIATE_TEMPLATES(Tool)
