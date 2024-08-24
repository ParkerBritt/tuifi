#ifndef APP_MENU_HPP
#define APP_MENU_HPP

#include "CompSearchBar.hpp"
#include <iostream>
#include <ftxui/component/component.hpp>
#include <string>
#include <vector>

namespace ui = ftxui;

namespace launcher {
ui::Component makeMenu(std::vector<std::string>& menuEntries, std::vector<std::shared_ptr<launcher::Application>>& visibleApplications, int& selectedEntry, std::shared_ptr<SearchBar>& input);
}

#endif
