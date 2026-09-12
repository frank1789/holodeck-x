#ifndef HOLODECK_X_APP_UI_PANELS_ANALYSIS_TREE_HH_
#define HOLODECK_X_APP_UI_PANELS_ANALYSIS_TREE_HH_

#include "ui/abstract_panel.hh"
#include <vector>
#include <string>

namespace holodeckx::ui {

class AnalysisTreePanel : public BasePanel<AnalysisTreePanel> {
public:
  struct Node {
    std::string label;
    std::vector<Node> children;
    void *user_data{nullptr};
  };

  AnalysisTreePanel() : BasePanel<AnalysisTreePanel>("Analysis Tree") {}

  auto draw_content() -> void;

private:
  auto draw_node(Node &node) -> void;

  std::vector<Node> roots_;
  Node *selected_{nullptr};
};

} // namespace holodeckx::ui

#endif // HOLODECK_X_APP_UI_PANELS_ANALYSIS_TREE_HH_
