#ifndef APP_MENU_HPP
#define APP_MENU_HPP

#include "CompSearchBar.hpp"
#include <iostream>
#include <ftxui/component/component.hpp>
#include <string>
#include <vector>

namespace ui = ftxui;

namespace launcher {
struct MenuData{
    std::vector<std::string> menuEntries;
    std::vector<std::shared_ptr<launcher::Application>> visibleApplications;
    int selectedEntry;
};
ui::Component makeMenu(MenuData& menuData, std::shared_ptr<SearchBar>& input, ftxui::ScreenInteractive& screen);
}

#endif
