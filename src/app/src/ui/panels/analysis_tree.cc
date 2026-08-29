#include "ui/panels/analysis_tree.hh"

#include <imgui.h>

namespace holodeckx::ui {

namespace {}

auto AnalysisTreePanel::draw_content() -> void {
  ImGui::PushStyleColor(ImGuiCol_TableRowBg,
                        IM_COL32(45, 45, 48, 255));  // dark-grey
  ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt,
                        IM_COL32(60, 60, 64, 255));  // grey

  constexpr ImGuiTableFlags table_falgs =
      ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV |
      ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoPadOuterX;

  const ImVec2 outer_size = ImGui::GetContentRegionAvail();
  if (ImGui::BeginTable("Tree", 1, table_falgs, outer_size)) {
    ImGui::TableSetupColumn("Item", ImGuiTableColumnFlags_WidthStretch);
    for (auto& root : roots_) {
      draw_node(root);
    }

    ImGui::EndTable();
  }
  ImGui::PopStyleColor(2);
}

auto AnalysisTreePanel::draw_node(Node& node) -> void {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth |
                             ImGuiTreeNodeFlags_OpenOnArrow |
                             ImGuiTreeNodeFlags_OpenOnDoubleClick;
  const bool is_leaf = node.children.empty();
  if (is_leaf) {
    flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
  }

  if (selected_ == &node) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  // to avoid label collision between same-name node
  // using the node's address as ID which should be stable across frames
  ImGui::PushID(&node);
  const bool open = ImGui::TreeNodeEx(node.label.c_str(), flags);
  if (ImGui::IsItemClicked()) {
    selected_ = &node;
  }

  ImGui::PopID();
  if (open && !is_leaf) {
    for (auto& child : node.children) {
      draw_node(child);
    }
    ImGui::TreePop();
  }
}

}  // namespace holodeckx::ui
