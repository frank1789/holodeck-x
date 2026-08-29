#ifndef HOLODECK_X_APP_UI_PANELS_OUTPUT_LOG_HH_
#define HOLODECK_X_APP_UI_PANELS_OUTPUT_LOG_HH_

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

class OutputLogPanel : public BasePanel<OutputLogPanel> {
 public:
  OutputLogPanel() : BasePanel<OutputLogPanel>("Output") {}

    auto draw_content() -> void;
};

}  // namespace holodeckx::ui

#endif  //    HOLODECK_X_APP_UI_PANELS_OUTPUT_LOG_HH_
