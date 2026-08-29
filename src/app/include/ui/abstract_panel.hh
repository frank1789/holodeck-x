#ifndef HOLODECK_X_APP_UI_ABSTRACT_PANEL_HH_
#define HOLODECK_X_APP_UI_ABSTRACT_PANEL_HH_

#include <string>
#include <imgui.h>

namespace holodeckx::ui {

class AbstractPanel {
public:
  virtual ~AbstractPanel() noexcept = default;

  //   AbstractPanel(const AbstractPanel&) = delete;
  //   AbstractPanel& operator=(const AbstractPanel&) = delete;

  //   AbstractPanel(AbstractPanel&&) noexcept = delete;
  //   AbstractPanel& operator=(AbstractPanel&&) noexcept = delete;

  virtual void draw() = 0;
  virtual void set_open(bool open) = 0;

  [[nodiscard]] virtual auto get_name() const -> const char * = 0;

  [[nodiscard]] virtual auto is_open() const -> bool = 0;

  //  private:
  //   constexpr AbstractPanel() noexcept = default;
};

template <typename Derived> class BasePanel : public AbstractPanel {
public:
  explicit BasePanel(std::string name, bool start_open = true)
      : name_(std::move(name)), open_(start_open) {}

  void draw() final {
    if (!open_) {
      return;
    }

    auto &self = static_cast<Derived &>(*this);
    if (auto *wc = GetWindowClassIfAny(self, 0)) {
      ImGui::SetNextWindowClass(wc);
    }
    const ImGuiWindowFlags flags = GetFlagsIfAny(self, 0);

    //
    //
    //
    struct ScopedWindow {
      explicit ScopedWindow(const char *title, bool *open,
                            ImGuiWindowFlags flags) {
        visible = ImGui::Begin(title, open, flags);
      }
      ~ScopedWindow() noexcept { ImGui::End(); }
      bool visible;
    } window(name_.c_str(), &open_, flags);

    if (window.visible) {
      self.draw_content();
    }
  }

  void set_open(bool open) final { open_ = open; }

  auto get_name() const -> const char * final { return name_.c_str(); }

  auto is_open() const -> bool final { return open_; }

protected:
  std::string name_;
  bool open_;

  private:
  // ---- SFINAE probes for optional Derived hooks -------------------
    // These let a panel opt into custom flags/window-class without every
    // panel having to define boilerplate it doesn't need.
    template <typename T>
    static auto GetFlagsIfAny(T& self, int)
        -> decltype(self.GetWindowFlags())
    {
        return self.GetWindowFlags();
    }
    template <typename T>
    static ImGuiWindowFlags GetFlagsIfAny(T&, long) // fallback, lower overload rank
    {
        return ImGuiWindowFlags_None;
    }

    template <typename T>
    static auto GetWindowClassIfAny(T& self, int)
        -> decltype(self.GetWindowClass())
    {
        return self.GetWindowClass();
    }
    template <typename T>
    static ImGuiWindowClass* GetWindowClassIfAny(T&, long)
    {
        return nullptr;
    }

};

} // namespace holodeckx::ui

#endif // HOLODECK_X_APP_UI_ABSTRACT_PANEL_HH_
