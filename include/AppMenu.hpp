#ifndef APP_MENU_HPP
#define APP_MENU_HPP

#include "CompSearchBar.hpp"
#include <ftxui/component/component.hpp>
#include "MenuData.hpp"

namespace ui = ftxui;

namespace launcher {
ui::Component makeMenu(MenuData& menuData, std::shared_ptr<SearchBar>& input, ftxui::ScreenInteractive& screen);
}

#endif
