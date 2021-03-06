#pragma once

#include "../../model/include/Session.h"

#include <Wt/WDialog.h>

class ModelFile;
class BendSequence;

namespace Wt {
    class WText;
    class WInPlaceEdit;
    class WSpinBox;
    class WTable;
    class WPushButton;
}

namespace Fxt {
    namespace Dao {
        class ProcessPlanDao;
    }
}

class ProcessPlanDialog : public Wt::WDialog
{
private:
    Session& session_;
    dbo::ptr<ModelFile> modelFile_;
    dbo::ptr<BendSequence> processPlan_;
    
    Wt::WInPlaceEdit* moderator_;
    Wt::WText* creationDate_;
    Wt::WInPlaceEdit* partNo_;
    Wt::WText* partName_;
    Wt::WText* material_;
    Wt::WSpinBox* quantity_;

    Wt::WText* nTools_;
    Wt::WText* thickness_;
    Wt::WText* bendingForce_;
    Wt::WText* nRotations_;
    Wt::WText* nFlips_;
    Wt::WText* processPlanningTime_;
    Wt::WText* totalProcessingTime_;

    Wt::WTable* bendSequenceTable_;
    Wt::WPushButton *ok_, *download_;

    inline void quantityChanged(unsigned nParts, unsigned nTools, unsigned nBends, unsigned nFlips, unsigned nRotations);
    inline void setProductionTime(const double time_p);
public:
    void setModelCrumb(dbo::ptr<ModelFile> modelFile);
    ProcessPlanDialog(Session& session, const std::string& title, Wt::Dbo::ptr<ModelFile>& modelFile_);
    ~ProcessPlanDialog();

    void ok();
    void cancel();
};