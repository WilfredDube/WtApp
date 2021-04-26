#pragma once

#include "../../model/include/Tool.h"
#include "../../model/include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<Tool> > Tools;

namespace Fxt 
{
namespace Dao
{
    class ToolDao
    {
    public:

        ToolDao(Session& session) : session { session } {}

        Wt::Dbo::ptr<Tool> insert(std::string toolId, 
                                std::string toolName, 
                                double toolAngle, 
                                double toolLength, 
                                double toolMinRadius, 
                                double toolMaxRadius);

        Wt::Dbo::ptr<Tool> get(double toolAngle);

        Tools getAll();

        bool deleteProject(Wt::Dbo::ptr<Tool>& tool);

    private:
        Session& session;
    };
}
}