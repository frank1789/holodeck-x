#include "ui/panels/main_panel.hh"

#include <ImViewGuizmo.h>
#include <fmt/format.h>
#include <imgui.h>

namespace holodeckx::ui {

auto MainPanel::draw_content() -> void {
  draw_scene_render_target();
  draw_view_gizmo_overlay();
}

auto MainPanel::get_window_flags() const -> ImGuiWindowFlags {
  return ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

auto MainPanel::draw_scene_render_target() -> void {
  const ImVec2 size = ImGui::GetContentRegionAvail();
  // placeholder real render backend
  ImGui::InvisibleButton("SceneCanvas", size);
  ImDrawList* dl = ImGui::GetWindowDrawList();
  const ImVec2 p0 = ImGui::GetItemRectMin();
  const ImVec2 p1 = ImGui::GetItemRectMax();
  dl->AddRectFilled(p0, p1, IM_COL32(28, 28, 32, 255));
}

auto MainPanel::draw_view_gizmo_overlay() -> void {
  if (!camera_) {
    return;
  }

  // anchor the gizmo to the top-right corner
  constexpr auto margin = 34.0f;
  constexpr auto gizmo_radius = 48.0f;
  constexpr auto zoom_spacing = 40.0f;
  constexpr auto pan_spacing = 40.0f;

  const ImVec2 window_size = ImGui::GetWindowSize();
  const ImVec2 window_position = ImGui::GetWindowPos();

  // 1. Top widget: Rotate (anchored to top-right corner)
  const ImVec2 rotate_pos{
      window_position.x + window_size.x - margin - gizmo_radius,
      window_position.y + margin + gizmo_radius};

  // 2. Middle widget: Zoom (shifted down by radius + spacing)
  const ImVec2 zoom_pos{rotate_pos.x,
                        rotate_pos.y + gizmo_radius + zoom_spacing};

  // 3. Bottom widget: Pan (shifted down again by radius + spacing)
  const ImVec2 pan_pos{rotate_pos.x, zoom_pos.y + pan_spacing};

  // Render the aligned widgets
  ImViewGuizmo::Rotate(camera_->position, camera_->rotation, {}, rotate_pos);
  ImViewGuizmo::Dolly(camera_->position, camera_->rotation, zoom_pos);
  ImViewGuizmo::Pan(camera_->position, camera_->rotation, pan_pos);

  is_gizmo_active_ = ImViewGuizmo::IsUsing() || ImViewGuizmo::IsOver();
}

}  // namespace holodeckx::ui
