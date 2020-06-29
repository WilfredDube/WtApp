#ifndef FEATURE_DIALOG_H_
#define FEATURE_DIALOG_H_

#include <Wt/WDialog.h>
#include "../model/Session.h"

class ModelFile;

namespace Wt {
    class WTable;
    class WPushButton;
}

class FeatureDialog : public Wt::WDialog
{
private:
    Session& session_;
    dbo::ptr<ModelFile> modelFile_;

    Wt::WTable* bendSequenceTable_;
    Wt::WPushButton *ok_;
public:
    FeatureDialog(Session& session, const std::string& title, Wt::Dbo::ptr<ModelFile>& modelFile_);
    ~FeatureDialog();

    void ok();
    void cancel();
};

#endif