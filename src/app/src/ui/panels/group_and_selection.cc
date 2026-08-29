#include "ui/panels/group_and_selection.hh"

#include <imgui.h>

namespace holodeckx::ui {

auto GroupAndSelectionPanel::draw_content() -> void {
  ImGui::TextDisabled("Selection sets / grouping UI goes here");
}

}  // namespace holodeckx::ui
