#include "../include/BendFeatureDao.h"
#include "../include/MaterialDao.h"
#include "../include/ToolDao.h"

#include <memory>

namespace dbo = Wt::Dbo;

using namespace Fxt::SheetMetalComponent;

Wt::Dbo::ptr<BendFeature> BendFeatureDao::insert(Bend::ModelBend& modelBend,
                                     dbo::ptr<ModelFile>& cadFile,
                                     double thickness)
{
    auto ret = Wt::Dbo::ptr<BendFeature>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<BendFeature> bendFeature(Wt::cpp14::make_unique<BendFeature>());

        bendFeature->bend_id = modelBend.getFaceId();
        bendFeature->face_id1 = modelBend.getBendFeature()->getJoiningFaceID1();
        bendFeature->face_id2 = modelBend.getBendFeature()->getJoiningFaceID2();
        bendFeature->bend_angle = modelBend.getBendFeature()->getBendAngle();
        bendFeature->bend_radius = modelBend.getBendFeature()->getBendRadius();
        bendFeature->bend_length = modelBend.getBendFeature()->getBendLength();
        bendFeature->modelFile = cadFile;
        bendFeature->bend_direction = 0;

        ToolDao toolDao { session };
        bendFeature->bending_tool_id = toolDao.get(bendFeature->bend_angle)->tool_id;

        MaterialDao materialDao { session };
        bendFeature->bend_force = materialDao.getBendingForce(cadFile->modelMaterial.toUTF8(), thickness, bendFeature->bend_length);
        
        ret = session.add(std::move(bendFeature));
    }
    
    return ret;
}

BendFeatures BendFeatureDao::get(dbo::ptr<ModelFile>& modelFile)
{
    dbo::Transaction transaction(session);
    
    BendFeatures bendFeatures = session.find<BendFeature>()
                                        .where("modelFile = ?").bind(modelFile);

    return bendFeatures;
}

BendFeatures BendFeatureDao::getAll()
{
    return session.find<BendFeature>();
}

bool BendFeatureDao::deleteProject(dbo::ptr<BendFeature>& bendFeature)
{
    dbo::Transaction transaction(session);

    bendFeature.remove();

    return true;
}