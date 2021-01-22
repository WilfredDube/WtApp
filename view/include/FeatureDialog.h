#pragma once

#include "../../model/include/Session.h"

#include <Wt/WDialog.h>

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