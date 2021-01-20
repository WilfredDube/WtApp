#pragma once

#include "../../model/include/BendSequence.h"
#include "../../model/include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<BendSequence> > BendSequences;

class BendSequenceDao
{
public:

    BendSequenceDao(Session& session) : session { session } {}

    void insert(std::vector<int> bendSequence,  dbo::ptr<ProcessPlan>& processPlan);

    BendSequences get(dbo::ptr<ModelFile>& modelFile);

    BendSequences getAll();

    bool deleteProject(Wt::Dbo::ptr<BendSequence>& material);

private:
    Session& session;
};