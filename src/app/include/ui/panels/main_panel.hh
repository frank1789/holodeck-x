#ifndef HOLODECK_X_APP_UI_PANELS_MAIN_PANEL_HH_
#define HOLODECK_X_APP_UI_PANELS_MAIN_PANEL_HH_

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

/// \brief A minimal camera model
struct Camera {
  glm::vec3 position{4.0f, 3.0f, 6.0f};
  glm::quat rotation =
      glm::quatLookAt(glm::normalize(-position), glm::vec3(0, 1, 0));
};

class MainPanel : public BasePanel<MainPanel> {
 public:
  MainPanel() : BasePanel<MainPanel>("Main") {}

  auto draw_content() -> void;
  auto set_camera(Camera* camera) -> void { camera_ = camera; }

  [[nodiscard]] auto get_window_flags() const -> ImGuiWindowFlags;

  [[nodiscard]] constexpr auto is_gizmo_capturing_input() const -> bool {
    return is_gizmo_active_;
  }

 private:
  auto draw_scene_render_target() -> void;
  auto draw_view_gizmo_overlay() -> void;

  Camera* camera_{nullptr};
  bool is_gizmo_active_{false};
};

}  // namespace holodeckx::ui

#endif  //
