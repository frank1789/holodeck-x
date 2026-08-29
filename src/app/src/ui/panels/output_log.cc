#include "ui/panels/output_log.hh"

#include <imgui.h>

namespace holodeckx::ui {

auto OutputLogPanel::draw_content() -> void {
  ImGui::TextDisabled("Selection sets / grouping UI goes here");
}

}  // namespace holodeckx::ui
