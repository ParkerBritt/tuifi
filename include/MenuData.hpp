#ifndef MENU_DATA_HPP
#define MENU_DATA_HPP

#include <string>
#include <vector>
#include <memory>
#include "Application.hpp"

namespace launcher {
struct MenuData{
    std::vector<std::string> menuEntries;
    std::vector<std::shared_ptr<launcher::Application>> visibleApplications;
    int selectedEntry;
};
}

#endif
