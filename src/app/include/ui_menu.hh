#ifndef UI_MENU_HH_
#define UI_MENU_HH_

#include <string_view>
#include <functional>
#include <span>

struct MenuItem {
    std::string_view label;
    std::string_view shortcut;
    std::function<int()> callback;
    std::function<bool()> checked;
    std::function<bool()> enabled;
    std::uint64_t id;
};

struct MenuNode {
    std::string_view label;
    // std::span<const MenuItem> items;
    // std::span<const MenuNode> children;
};

#endif // UI_MENU_HH_
