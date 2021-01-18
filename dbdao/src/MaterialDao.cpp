#include "../include/MaterialDao.h"

#include <memory>

namespace dbo = Wt::Dbo;

Wt::Dbo::ptr<Material> MaterialDao::insert(std::string material_name, double tensile_strength, double k_factor)
{
    auto ret = Wt::Dbo::ptr<Material>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<Material> material(Wt::cpp14::make_unique<Material>());
        material->material_name = (material_name);
        material->tensile_strength = (tensile_strength);
        material->k_factor = (k_factor);
        
        ret = session.add(std::move(material));
    }
    
    return ret;
}

dbo::ptr<Material> MaterialDao::get(std::string materialName)
{
    dbo::Transaction transaction(session);
    
    dbo::ptr<Material> material = session.query<dbo::ptr<Material>>("select m from material m")
                                        .where("material_name = ?").bind(materialName);

    return material;
}

Materials MaterialDao::getAll()
{
    return session.find<Material>();
}

bool MaterialDao::deleteProject(dbo::ptr<Material>& material)
{
    dbo::Transaction transaction(session);

    material.remove();

    return true;
}