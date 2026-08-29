#ifndef HOLODECK_X_APP_UI_PANEL_MANANGER_HH_
#define HOLODECK_X_APP_UI_PANEL_MANANGER_HH_

#include <memory>
#include <span>
#include <vector>

#include "ui/abstract_panel.hh"

namespace holodeckx::ui {

class PanelManager {
 public:
  using panel_ptr = std::unique_ptr<AbstractPanel>;

  template<typename T, typename... Args>
  auto add_panel(Args&&... args) -> T& {
    auto owned = std::make_unique<T>(std::forward<Args>(args)...);
    T& ref = *owned;
    panels_.push_back(std::move(owned));
    return ref;
  }

  auto draw_all() -> void {
        {
        for (auto& panel : panels_)
            panel->draw();

        // Sweep out panels that were closed this frame, if desired.
        // (Comment out if you want closed panels to persist for later reopening.)
        panels_.erase(
            std::remove_if(panels_.begin(), panels_.end(),
                            [](const std::unique_ptr<AbstractPanel>& p) { return !p->is_open(); }),
            panels_.end());
    }
  }

  [[nodiscard]] auto panels() const -> std::span<const panel_ptr> {
    return panels_;
  }

 private:
  std::vector<panel_ptr> panels_{};
};

}  // namespace holodeckx::ui

#endif  // HOLODECK_X_APP_UI_PANEL_MANANGER_HH_
