#pragma once

#include "../../model/include/Material.h"
#include "../../model/include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<Material> > Materials;

class MaterialDao
{
public:

    MaterialDao(Session& session) : session { session } {}

    Wt::Dbo::ptr<Material> insert(std::string material_name, double tensile_strength, double k_factor);

    Wt::Dbo::ptr<Material> get(std::string material_name);

    Materials getAll();

    bool deleteProject(Wt::Dbo::ptr<Material>& material);

private:
    Session& session;
};