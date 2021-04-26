#pragma once

#include <Wt/WContainerWidget.h>

namespace Wt {
  class WPanel;
}

class PanelList : public Wt::WContainerWidget
{
public:
  PanelList();

  Wt::WPanel *addWidget(const Wt::WString& text, std::unique_ptr<Wt::WWidget> w);
  void addPanel(std::unique_ptr<Wt::WPanel> panel);
  void removePanel(Wt::WPanel *panel);
  void react();

  using WContainerWidget::addWidget;

private:
  void onExpand(bool notUndo, Wt::WPanel *panel);

  int wasExpanded_;
};