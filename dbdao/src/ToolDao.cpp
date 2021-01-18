#include "../include/ToolDao.h"

#include <memory>

namespace dbo = Wt::Dbo;

Wt::Dbo::ptr<Tool> ToolDao::insert(std::string toolId, std::string toolName, double toolAngle, 
                                    double toolLength, double toolMinRadius, double toolMaxRadius)
{
    auto ret = Wt::Dbo::ptr<Tool>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<Tool> tool(Wt::cpp14::make_unique<Tool>());
        tool->tool_id = (toolId);
        tool->tool_name = (toolName);
        tool->tool_angle = (toolAngle);
        tool->tool_length = (toolLength);
        tool->tool_min_radius = (toolMinRadius);
        tool->tool_max_radius = (toolMaxRadius);
        
        ret = session.add(std::move(tool));
    }
    
    return ret;
}

dbo::ptr<Tool> ToolDao::get(double toolAngle)
{
    dbo::Transaction transaction(session);
    
    dbo::ptr<Tool> tool = session.query<dbo::ptr<Tool>>("select t from tool t")
                                        .where("tool_angle = ?").bind(toolAngle);

    return tool;
}

Tools ToolDao::getAll()
{
    return session.find<Tool>();
}

bool ToolDao::deleteProject(dbo::ptr<Tool>& tool)
{
    dbo::Transaction transaction(session);

    tool.remove();

    return true;
}