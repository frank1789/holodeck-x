#ifndef HOLODECK_X_APP_UI_PANELS_GROUP_AND_SELECTION_HH_
#define HOLODECK_X_APP_UI_PANELS_GROUP_AND_SELECTION_HH_

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

class GroupAndSelectionPanel : public BasePanel<GroupAndSelectionPanel> {
 public:
  GroupAndSelectionPanel()
      : BasePanel<GroupAndSelectionPanel>("Group & Selection") {}

  auto draw_content() -> void;
};

}  // namespace holodeckx::ui

#endif  //    HOLODECK_X_APP_UI_PANELS_GROUP_AND_SELECTION_HH_
