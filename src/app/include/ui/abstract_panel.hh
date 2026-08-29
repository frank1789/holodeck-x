#ifndef HOLODECK_X_APP_UI_ABSTRACT_PANEL_HH_
#define HOLODECK_X_APP_UI_ABSTRACT_PANEL_HH_

#include <string>

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

  [[nodiscard]] virtual auto get_name() const -> const char* = 0;

  [[nodiscard]] virtual auto is_open() const -> bool = 0;

//  private:
//   constexpr AbstractPanel() noexcept = default;
};

template <typename Derived>
class BasePanel : public AbstractPanel {
 public:
  explicit BasePanel(std::string name, bool start_open = true)
      : name_(std::move(name)), open_(start_open) {}

  void draw() final {
    if (!open_) {
      return;
    }

    auto derived = static_cast<Derived&>(*this);
    // if(){}
  }

  void set_open(bool open) final { open_ = open; }

  auto get_name() const  -> const char* final { return name_.c_str(); }

  auto is_open() const  -> bool final { return open_; }

 protected:
  std::string name_;
  bool open_;
};

}  // namespace holodeckx::ui

#endif  // HOLODECK_X_APP_UI_ABSTRACT_PANEL_HH_
