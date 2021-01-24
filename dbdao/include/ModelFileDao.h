#pragma once

#include "../../model/include/ModelFile.h"
#include "../../model/include/Session.h"

#include <Wt/WDate.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Types.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include <memory>

typedef Wt::Dbo::collection< Wt::Dbo::ptr<ModelFile> > ModelFiles;

namespace Fxt 
{
namespace Dao
{
    class ModelFileDao
    {
    public:

        ModelFileDao(Session& session) : session { session } {}

        Wt::Dbo::ptr<ModelFile> insert(std::string materialType, 
                                        std::string modelFileName,
                                        std::string modelObjFileName,
                                        std::string modelFileDir,
                                        dbo::ptr<Project> project);

        Wt::Dbo::ptr<ModelFile> get(std::string material_name);

        Wt::Dbo::ptr<ModelFile> update(dbo::ptr<ModelFile>& modelFile, dbo::ptr<ProcessPlan>& processPlan);
        Wt::Dbo::ptr<ModelFile> update(dbo::ptr<ModelFile>& modelFile, ProcessLevel processLevel);
        Wt::Dbo::ptr<ModelFile> update(dbo::ptr<ModelFile>& modelFile, double thickness);
        Wt::Dbo::ptr<ModelFile> update(dbo::ptr<ModelFile>& modelFile, float bendingForce);
        Wt::Dbo::ptr<ModelFile> update(dbo::ptr<ModelFile>& modelFile, long double feature_recognition_time);
        Wt::Dbo::ptr<ModelFile> update(dbo::ptr<ModelFile>& modelFile, std::string modelDataStringified);

        std::string getStringifiedModelData(dbo::ptr<ModelFile>& modelFile);

        ModelFiles getAll();

        bool deleteProject(Wt::Dbo::ptr<ModelFile>& cadFile);

    private:
        Session& session;
    };
}
}