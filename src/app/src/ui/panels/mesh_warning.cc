
#include "ui/panels/mesh_warning.hh"

#include <imgui.h>

namespace holodeckx::ui {

auto MeshWarningPanel::draw_content() -> void {
  ImGui::TextDisabled("Mesh Warning info goes here");
}

} // namespace holodeckx::ui
