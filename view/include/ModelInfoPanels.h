#pragma once

#include "../../model/include/Session.h"

#include <Wt/WContainerWidget.h>

namespace Wt {
    class WTable;
    class WGroupBox;
}

class PanelList;
class Project;
class ModelFile;

class ModelInfoPanels : public Wt::WContainerWidget
{
private:
    Session& session_;
    dbo::ptr<Project> project_;

    Wt::WGroupBox *modelFilesGroupBox_;
    Wt::WTable *faceRelationshipTable_;
    Wt::WTable *bendFeaturesSummaryTable_;
    // Wt::WGroupBox *processPlanGroupBox_;
    PanelList *panelList_;
    
    void reloadList();
    void updateModelList(dbo::ptr<ModelFile> modelFile);
    void updateFeatureTables(dbo::ptr<ModelFile> modelFile);
    void loadModelList();
    void loadFeatures();
    void loadfeatureSummary();
    void addProcessPlan();
public:
    ModelInfoPanels(Session& session, dbo::ptr<Project> project);
    ~ModelInfoPanels(){}
};