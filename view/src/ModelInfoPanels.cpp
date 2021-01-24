#include "../include/ModelInfoPanels.h"
#include "../include/ModelFileWidget.h"
#include "../include/PanelList.h"

#include "../../dbdao/include/ProjectDao.h"
#include "../../dbdao/include/ModelFileDao.h"

#include <Wt/WTable.h>
#include <Wt/WGroupBox.h>
#include <Wt/WText.h>

#include <cmath>

ModelInfoPanels::ModelInfoPanels(Session& session, dbo::ptr<Project> project)
    : WContainerWidget(),
    session_(session),
    project_(project)    
{
    auto panelList = Wt::cpp14::make_unique<PanelList>();
    panelList_ = panelList.get();

    addWidget(std::move(panelList));

    session_.modelFileChanged().connect(this, &ModelInfoPanels::updateModelList);
    session_.modelFileDeleted().connect(this, &ModelInfoPanels::reloadList);
    session_.modelFeaturesExtracted().connect(this, &ModelInfoPanels::updateFeatureTables);

    /************************** Model File List *********************/
    auto modelFilesGroupBox = Wt::cpp14::make_unique<Wt::WGroupBox>();
    modelFilesGroupBox_ = modelFilesGroupBox.get();
    modelFilesGroupBox->addStyleClass("centered-example");
    auto mf = panelList_->addWidget("Project Model Files", std::move(modelFilesGroupBox));
    loadModelList();
  
    /************************** Face relationship table *********************/
    auto faceRelationshipTable = Wt::cpp14::make_unique<Wt::WTable>();
    faceRelationshipTable_ = faceRelationshipTable.get();
    faceRelationshipTable->setHeaderCount(1);
    faceRelationshipTable->setWidth(Wt::WLength("100%"));
    faceRelationshipTable->elementAt(0, 0)->addNew<Wt::WText>("Face ID");
    faceRelationshipTable->elementAt(0, 1)->addNew<Wt::WText>("Bend ID");
    faceRelationshipTable->elementAt(0, 2)->addNew<Wt::WText>("Face ID");
    faceRelationshipTable->setStyleClass("table-bordered table-hover table-striped");
    panelList_->addWidget("Face Relationships", std::move(faceRelationshipTable));

    /************************** Bend Feature Summary *********************/
    auto bendFeaturesSummaryTable = Wt::cpp14::make_unique<Wt::WTable>();
    bendFeaturesSummaryTable_ = bendFeaturesSummaryTable.get();
    bendFeaturesSummaryTable->setHeaderCount(1);
    bendFeaturesSummaryTable->setWidth(Wt::WLength("100%"));
    bendFeaturesSummaryTable->elementAt(0, 0)->addNew<Wt::WText>("Bend ID");
    bendFeaturesSummaryTable->elementAt(0, 1)->addNew<Wt::WText>("Angle");
    bendFeaturesSummaryTable->elementAt(0, 2)->addNew<Wt::WText>("Length");
    bendFeaturesSummaryTable->elementAt(0, 3)->addNew<Wt::WText>("Radius");
    bendFeaturesSummaryTable->setStyleClass("table-bordered table-hover table-striped");
    panelList_->addWidget("Bend Feature Summary", std::move(bendFeaturesSummaryTable));

    /************************** Project Process Plans *********************/
    auto processPlanGroupBox = Wt::cpp14::make_unique<Wt::WGroupBox>();
    processPlanGroupBox_ = processPlanGroupBox.get();
    processPlanGroupBox->addStyleClass("centered-example");
    panelList_->addWidget("Project Process Plans", std::move(processPlanGroupBox));
}

void ModelInfoPanels::loadModelList()
{
    ModelFiles modelFiles = project_->getModelFiles();
    for (auto a : modelFiles)
    {
        modelFilesGroupBox_->addNew<ModelFileWidget>(session_, a);;
    } 
}

void ModelInfoPanels::reloadList()
{
    // dbo::Transaction t(session_);

    // project_.modify()->nModelFiles = project_->modelFiles.size() - 1;    

    // t.commit();
}

void ModelInfoPanels::updateModelList(dbo::ptr<ModelFile> modelFile)
{   
    // dbo::Transaction t(session_);

    // project_.modify()->nModelFiles = project_->modelFiles.size();    

    // t.commit();

    modelFilesGroupBox_->addNew<ModelFileWidget>(session_, modelFile);
}

void ModelInfoPanels::updateFeatureTables(dbo::ptr<ModelFile> modelFile)
{
    if (modelFile->getProcessLevel() == ProcessLevel::UNPROCESSED)
    {
        ; // Do nothing
    } else {
        dbo::Transaction t(session_);

        BendFeatures bf = modelFile->getBendFeatures();

        faceRelationshipTable_->clear();
        faceRelationshipTable_->setHeaderCount(1);
        faceRelationshipTable_->setWidth(Wt::WLength("100%"));
        faceRelationshipTable_->elementAt(0, 0)->addNew<Wt::WText>("Face ID");
        faceRelationshipTable_->elementAt(0, 1)->addNew<Wt::WText>("Bend ID");
        faceRelationshipTable_->elementAt(0, 2)->addNew<Wt::WText>("Face ID");
        faceRelationshipTable_->setStyleClass("table-bordered table-hover table-striped");

        bendFeaturesSummaryTable_->clear();
        bendFeaturesSummaryTable_->setHeaderCount(1);
        bendFeaturesSummaryTable_->setWidth(Wt::WLength("100%"));
        bendFeaturesSummaryTable_->elementAt(0, 0)->addNew<Wt::WText>("Bend ID");
        bendFeaturesSummaryTable_->elementAt(0, 1)->addNew<Wt::WText>("Angle");
        bendFeaturesSummaryTable_->elementAt(0, 2)->addNew<Wt::WText>("Length");
        bendFeaturesSummaryTable_->elementAt(0, 3)->addNew<Wt::WText>("Radius");
        bendFeaturesSummaryTable_->setStyleClass("table-bordered table-hover table-striped");

        int rowCount = faceRelationshipTable_->rowCount();
        int summaryRowCount = bendFeaturesSummaryTable_->rowCount();
        for(auto a: bf) {
            faceRelationshipTable_->elementAt(rowCount, 0)->addNew<Wt::WText>(std::to_string(a->face_id1));
            faceRelationshipTable_->elementAt(rowCount, 1)->addNew<Wt::WText>(std::to_string(a->bend_id));
            faceRelationshipTable_->elementAt(rowCount, 2)->addNew<Wt::WText>(std::to_string(a->face_id2));
            ++rowCount;

            bendFeaturesSummaryTable_->elementAt(summaryRowCount, 0)->addNew<Wt::WText>(std::to_string(a->bend_id));
            bendFeaturesSummaryTable_->elementAt(summaryRowCount, 1)->addNew<Wt::WText>(std::to_string(std::round(a->bend_angle)));
            bendFeaturesSummaryTable_->elementAt(summaryRowCount, 2)->addNew<Wt::WText>(std::to_string(std::round(a->bend_length)));
            bendFeaturesSummaryTable_->elementAt(summaryRowCount, 3)->addNew<Wt::WText>(std::to_string(std::round(a->bend_radius)));
            ++summaryRowCount;
        }

        t.commit();
    }
    
}