#pragma once

#include "../../model/include/ModelFile.h"

#include <Wt/WContainerWidget.h>

class ColorWidget : public Wt::WContainerWidget
{
public:
    ColorWidget();
    void setIconColor(ProcessLevel pL);
};