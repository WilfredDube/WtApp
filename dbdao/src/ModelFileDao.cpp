#include "../include/ModelFileDao.h"

#include <memory>

namespace dbo = Wt::Dbo;

using namespace Fxt::Dao;

Wt::Dbo::ptr<ModelFile> ModelFileDao::insert(std::string materialType, 
                                             std::string modelFileName,
                                             std::string modelObjFileName,
                                             std::string modelFileDir,
                                             dbo::ptr<Project> project)
{
    auto ret = Wt::Dbo::ptr<ModelFile>();

    {
        dbo::Transaction transaction(session);

        std::unique_ptr<ModelFile> cadFile(Wt::cpp14::make_unique<ModelFile>());
        cadFile->modelMaterial = materialType;
        cadFile->modelFile = modelFileName;
        cadFile->modelObjFile = modelObjFileName;
        cadFile->modelFileDir = modelFileDir;
        cadFile->project = project;

        cadFile->processLevel = ProcessLevel::UNPROCESSED;
        cadFile->author = session.user();
        cadFile->uploadDate = Wt::WDateTime::currentDateTime();

        cadFile->project.modify()->no_files++;
        
        ret = session.add(std::move(cadFile));
    }
    
    return ret;
}

dbo::ptr<ModelFile> ModelFileDao::update(dbo::ptr<ModelFile>& modelFile, dbo::ptr<ProcessPlan>& processPlan)
{
    {
        dbo::Transaction transaction(session);

        modelFile.modify()->processPlan = processPlan;
    }

    return modelFile;
}

dbo::ptr<ModelFile> ModelFileDao::update(dbo::ptr<ModelFile>& modelFile, ProcessLevel processLevel)
{
    {
        dbo::Transaction transaction(session);

        modelFile.modify()->processLevel = processLevel;

        if(processLevel == ProcessLevel::FEATURE_EXTRACTED)
            modelFile.modify()->nbends = modelFile->bendFeatures.size();
    }

    return modelFile;
}

dbo::ptr<ModelFile> ModelFileDao::update(dbo::ptr<ModelFile>& modelFile, float bendingForce)
{
    {
        dbo::Transaction transaction(session);

        modelFile.modify()->bendingForce = bendingForce;
    }

    return modelFile;
}

dbo::ptr<ModelFile> ModelFileDao::update(dbo::ptr<ModelFile>& modelFile, double thickness)
{
    {
        dbo::Transaction transaction(session);

        modelFile.modify()->thickness = thickness;
    }

    return modelFile;
}

dbo::ptr<ModelFile> ModelFileDao::update(dbo::ptr<ModelFile>& modelFile, std::string modelDataStringified)
{
    {
        dbo::Transaction transaction(session);

        modelFile.modify()->modelData = modelDataStringified;
    }

    return modelFile;
}

dbo::ptr<ModelFile> ModelFileDao::update(dbo::ptr<ModelFile>& modelFile, long double feature_recognition_time)
{
    {
        dbo::Transaction transaction(session);

        modelFile.modify()->feature_recognition_time = feature_recognition_time;
    }

    return modelFile;
}

dbo::ptr<ModelFile> ModelFileDao::get(std::string materialName)
{
    dbo::Transaction transaction(session);
    
    dbo::ptr<ModelFile> cadFile = session.query<dbo::ptr<ModelFile>>("select cadFile from model cadFile")
                                        .where("material_name = ?").bind(materialName);

    return cadFile;
}

std::string ModelFileDao::getStringifiedModelData(dbo::ptr<ModelFile>& modelFile)
{
   return modelFile->getModelData().toUTF8();
}

ModelFiles ModelFileDao::getAll()
{
    return session.user()->modelfiles;
}

bool ModelFileDao::deleteProject(dbo::ptr<ModelFile>& cadFile)
{
    dbo::Transaction transaction(session);

    cadFile.remove();

    return true;
}