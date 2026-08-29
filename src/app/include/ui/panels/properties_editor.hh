#ifndef HOLODECK_X_APP_UI_PANELS_PROPERTIES_EDITOR_HH_
#define HOLODECK_X_APP_UI_PANELS_PROPERTIES_EDITOR_HH_

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

class PropertiesEditorPanel : public BasePanel<PropertiesEditorPanel> {
 public:
  PropertiesEditorPanel()
      : BasePanel<PropertiesEditorPanel>("Property Editor") {}

  auto draw_content() -> void;
};

}  // namespace holodeckx::ui

#endif  //    HOLODECK_X_APP_UI_PANELS_PROPERTIES_EDITOR_HH_
