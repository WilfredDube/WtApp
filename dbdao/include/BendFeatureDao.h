#pragma once

#include "../../model/include/BendFeature.h"
#include "../../model/include/Session.h"

#include "../../libfxtract/include/sheet-metal-component/bend/ModelBend.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<BendFeature> > BendFeatures;

namespace Fxt 
{
namespace Dao
{
    class BendFeatureDao
    {
    public:

        BendFeatureDao(Session& session) : session { session } {}

        Wt::Dbo::ptr<BendFeature> insert(Fxt::SheetMetalComponent::Bend::ModelBend& modelFile,
                                        dbo::ptr<ModelFile>& cadFile,
                                        double thickness);

        Wt::Dbo::ptr<BendFeature> update(dbo::ptr<BendFeature>& bendFeature);

        BendFeatures get(dbo::ptr<ModelFile>& modelFile);

        BendFeatures getAll();

        bool deleteProject(Wt::Dbo::ptr<BendFeature>& bendFeature);

    private:
        Session& session;
    };
}
}