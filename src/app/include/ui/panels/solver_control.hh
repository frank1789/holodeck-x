#ifndef HOLODECK_X_APP_UI_PANELS_SOLVER_CONTROL_HH_
#define HOLODECK_X_APP_UI_PANELS_SOLVER_CONTROL_HH_

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

class SolverControlPanel : public BasePanel<SolverControlPanel> {
 public:
  SolverControlPanel() : BasePanel<SolverControlPanel>("Solver Control") {}

    auto draw_content() -> void;

};
}  // namespace holodeckx::ui
#endif  //    HOLODECK_X_APP_UI_PANELS_SOLVER_CONTROL_HH_
