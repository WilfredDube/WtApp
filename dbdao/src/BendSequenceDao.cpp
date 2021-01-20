#include "../include/BendSequenceDao.h"

#include <cmath>
#include <memory>

namespace dbo = Wt::Dbo;

void BendSequenceDao::insert(std::vector<int> bendSequence, dbo::ptr<ProcessPlan>& processPlan)
{
    auto ret = Wt::Dbo::ptr<BendSequence>();

    {
        dbo::Transaction transaction(session);

        for(const auto& bs : bendSequence)
        {
            std::unique_ptr<BendSequence> bendSeq(Wt::cpp14::make_unique<BendSequence>());
            
            bendSeq->bend_id = bs;
            bendSeq->processPlan = processPlan;

            session.add(std::move(bendSeq));
        }        
    }    
}

BendSequences BendSequenceDao::get(dbo::ptr<ModelFile>& modelFile)
{
    dbo::Transaction transaction(session);

    return modelFile->processPlan->bendSequences;
}

BendSequences BendSequenceDao::getAll()
{
    return session.find<BendSequence>();
}

bool BendSequenceDao::deleteProject(dbo::ptr<BendSequence>& material)
{
    dbo::Transaction transaction(session);

    material.remove();

    return true;
}