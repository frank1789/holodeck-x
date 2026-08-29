#ifndef HOLODECK_X_APP_UI_DOCKSPACE_HOST_HH_
#define HOLODECK_X_APP_UI_DOCKSPACE_HOST_HH_

#include <imgui.h>

namespace holodeckx::ui {

class DockSpaceHost {
 public:
  static auto begin(const char* dockspace_name = "MainDockSpace") -> void;

  static auto end() -> void;

 private:
  static auto build_default_layout(ImGuiID dockspace_id) -> void;

  inline static bool build_default_layout_ = false;
};

}  // namespace holodeckx::ui

#endif  // HOLODECK_X_APP_UI_DOCKSPACE_HOST_HH_
