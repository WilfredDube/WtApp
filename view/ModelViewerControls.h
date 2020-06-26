#ifndef MODEL_VIEWER_CONTROLS_H_
#define MODEL_VIEWER_CONTROLS_H_

#include <Wt/WContainerWidget.h>
#include "../model/Session.h"

namespace Wt {
    class WPushButton;
}

class Project;
class ModelFile;

class ModelViewerControls : public Wt::WContainerWidget
{
private:
    Session& session_;
    std::string uploadPath_;
    dbo::ptr<Project> project_;
    dbo::ptr<ModelFile> modelFile_;

    Wt::WPushButton *addModelButton_;
    Wt::WPushButton *bendingSeqButton_;
    Wt::WPushButton *featureButton_;
    Wt::WPushButton *SettingButton_;
    Wt::WTable *bendFeaturesTable_;
    
    void addModelDialog();
    void saveNewModel(std::vector<std::string> files, std::string materialType);
    void updateProjectModel(dbo::ptr<ModelFile> modelFile);
    void bendingSeqDialog();
    void featureDialog();
    void settingsDialog();
public:
    ModelViewerControls(Session& session, const std::string uploadPath, dbo::ptr<Project> project);
    ~ModelViewerControls();
};

#endif