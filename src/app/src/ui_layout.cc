
#include <imgui_internal.h>

void setup_default_layout(ImGuiID dockspace_id) {
  ImGui::DockBuilderRemoveNode(dockspace_id);
  ImGui::DockBuilderAddNode(dockspace_id,
                            ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

  ImGuiID dock_main_id = dockspace_id;
  ImGuiID dock_left_id;
  ImGuiID dock_right_id;
  ImGuiID dock_bottom_id;

  constexpr auto bottom_space_ratio{0.20f};
  constexpr auto left_space_ratio{0.22f};
  constexpr auto right_space_ratio{0.25};

  // split the bottom panel at 20% of vertical height
  dock_main_id =
      ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down,
                                  bottom_space_ratio, nullptr, &dock_bottom_id);

  // split the left panel at 22% of remaining horizontal width
  dock_main_id = ImGui::DockBuilderSplitNode(
      dock_main_id, ImGuiDir_Left, left_space_ratio, nullptr, &dock_left_id);

  // split the left panel at 25% of remaining horizontal width
  dock_main_id = ImGui::DockBuilderSplitNode(
      dock_main_id, ImGuiDir_Right, right_space_ratio, nullptr, &dock_right_id);

  // split the left column into Tree and Group
  ImGuiID dock_left_upper_id;
  ImGuiID dock_left_lower_id;
  ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Up, 0.60f, nullptr,
                              &dock_left_upper_id);
  ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Down, 0.40f, nullptr,
                              &dock_left_lower_id);

  // split the right column into Properties and Solver
  ImGuiID dock_right_upper_id;
  ImGuiID dock_right_lower_id;
  ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Up, 0.65f, nullptr,
                              &dock_right_upper_id);
  ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down, 0.35f, nullptr,
                              &dock_right_lower_id);

  // bind window string identifiers directly to the leaf nodes
  ImGui::DockBuilderDockWindow("Analysis Tree", dock_left_upper_id);
  ImGui::DockBuilderDockWindow("Group & Selection", dock_left_lower_id);
  ImGui::DockBuilderDockWindow("Property Editor", dock_right_upper_id);
  ImGui::DockBuilderDockWindow("Solver Control", dock_right_lower_id);

  ImGui::DockBuilderDockWindow("Output", dock_bottom_id);
  ImGui::DockBuilderDockWindow("Mesh Warning", dock_bottom_id);

  ImGui::DockBuilderDockWindow("Main", dock_main_id);
  ImGui::DockBuilderFinish(dockspace_id);
}
