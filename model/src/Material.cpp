#include "../include/Material.h"

#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

void Material::addMaterial(std::string material, double ts, double k)
{
  material_name = material;
  tensile_strength = ts;
  k_factor = k;
}

DBO_INSTANTIATE_TEMPLATES(Material)
