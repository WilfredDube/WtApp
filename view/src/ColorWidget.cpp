#include "../include/ColorWidget.h"

ColorWidget::ColorWidget() : Wt::WContainerWidget()
{
    setMaximumSize(10, 10);
    setPadding(10);
    addStyleClass("model-color-red", true);
}

void ColorWidget::setIconColor(ProcessLevel pL) 
{
    switch (pL)
    {
    case ProcessLevel::FEATURE_EXTRACTED:
        addStyleClass("model-color-orange", true);
        break;
    case ProcessLevel::PROCESS_PLAN_GEN:
        addStyleClass("model-color-green", true);
        break;
    default:
        break;
    }
}