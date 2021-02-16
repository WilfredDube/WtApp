#include "../include/FeatureDialog.h"

#include "../../dbdao/include/ModelFileDao.h"

#include <Wt/WPushButton.h>
#include <Wt/WTable.h>
#include <Wt/WComboBox.h>

namespace {

    auto processString = [](double x) {
        std::ostringstream os;
        os << x;
        return os.str();
    };
}

FeatureDialog::FeatureDialog(Session& session, const std::string& title, Wt::Dbo::ptr<ModelFile>& modelFile)
    : Wt::WDialog(title),
    session_(session),
    modelFile_(modelFile)
{
    ok_ = footer()->addNew<Wt::WPushButton>("DONE");

    rejectWhenEscapePressed();

    bendSequenceTable_ = contents()->addNew<Wt::WTable>();
    bendSequenceTable_->setHeaderCount(1);
    bendSequenceTable_->setWidth(Wt::WLength("100%"));
    bendSequenceTable_->elementAt(0, 0)->addNew<Wt::WText>("Bend ID");
    bendSequenceTable_->elementAt(0, 1)->addNew<Wt::WText>("Face ID");
    bendSequenceTable_->elementAt(0, 2)->addNew<Wt::WText>("Face ID");
    bendSequenceTable_->elementAt(0, 3)->addNew<Wt::WText>("Bend Angle");
    bendSequenceTable_->elementAt(0, 4)->addNew<Wt::WText>("Bend Length");
    bendSequenceTable_->elementAt(0, 5)->addNew<Wt::WText>("Bend Radius");
    bendSequenceTable_->elementAt(0, 6)->addNew<Wt::WText>("Bend Directions");
    bendSequenceTable_->elementAt(0, 7)->addNew<Wt::WText>("Tool Name");
    bendSequenceTable_->setStyleClass("myTable");

    dbo::Transaction t(session_);

    BendFeatures bf = modelFile_->getBendFeatures();

    int rowCount = bendSequenceTable_->rowCount();

    for(auto& bend : bf) {
        bendSequenceTable_->elementAt(rowCount, 0)->addNew<Wt::WText>("B" + std::to_string(bend->bend_id));
        bendSequenceTable_->elementAt(rowCount, 1)->addNew<Wt::WText>("F" + std::to_string(bend->face_id1));
        bendSequenceTable_->elementAt(rowCount, 2)->addNew<Wt::WText>("F" + std::to_string(bend->face_id2));
        bendSequenceTable_->elementAt(rowCount, 3)->addNew<Wt::WText>(processString(bend->bend_angle));
        bendSequenceTable_->elementAt(rowCount, 4)->addNew<Wt::WText>(processString(bend->bend_length));
        bendSequenceTable_->elementAt(rowCount, 5)->addNew<Wt::WText>(processString(bend->bend_radius));
        
        std::string bendDirection;
        if(bend->bend_direction == 1)
            bendDirection = "INSIDE";
        else if(bend->bend_direction == 2)
            bendDirection = "OUTSIDE";

        bendSequenceTable_->elementAt(rowCount, 6)->addNew<Wt::WText>(bendDirection);

        bendSequenceTable_->elementAt(rowCount, 7)->addNew<Wt::WText>(bend->bending_tool_id.c_str());

        ++rowCount;
    }

    t.commit();

    /*
     * Reject the dialog
     */
    ok_->clicked().connect(this, &Wt::WDialog::reject);    
}

FeatureDialog::~FeatureDialog(){}