#ifndef HOLODECK_X_APP_UI_PANELS_MESH_WARNING_HH_
#define HOLODECK_X_APP_UI_PANELS_MESH_WARNING_HH_

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

class MeshWarningPanel : public BasePanel<MeshWarningPanel> {
 public:
  MeshWarningPanel() : BasePanel<MeshWarningPanel>("Mesh Warning") {}

  auto draw_content() -> void;
};

}  // namespace holodeckx::ui

#endif  //    HOLODECK_X_APP_UI_PANELS_MESH_WARNING_HH_
