#include "AppMenu.hpp"
#include <cstdlib>
#include <unistd.h>
#include <ftxui/component/screen_interactive.hpp>


ui::Component launcher::makeMenu(MenuData& menuData, std::shared_ptr<SearchBar>& input, ftxui::ScreenInteractive& screen){
    ui::MenuOption menuOptions = ui::MenuOption::Vertical();

    menuOptions.entries_option.transform = [](ui::EntryState state) {
        // set colors
        ui::Color bgSelected = ui::Color::Purple3;
        ui::Color bgActive = ui::Color::Grey19;

        // set side icons
        ui::Element leftSide = ui::text((state.active ? "" : " "));
        ui::Element rightSide = ui::text((state.active ? "" : " "));

        // combine elements
        ui::Element menuStyle = hbox(leftSide, ui::text(state.label), rightSide);

        // style for focused item
        if (state.focused) {
          menuStyle = hbox(leftSide | color(bgSelected), ui::text(state.label) | bgcolor(bgSelected), rightSide | color(bgSelected));
        }

        // style for active item
        else if (state.active) {
          menuStyle = hbox(leftSide | color(bgActive), ui::text(state.label) | bgcolor(bgActive), rightSide | color(bgActive));
        }

        return menuStyle;
    };

    auto menu = ui::Menu(&menuData.menuEntries, &menuData.selectedEntry, menuOptions);
    menu |= ui::CatchEvent([&](ui::Event event){
        if(event.is_character() || event == ui::Event::Backspace){
            input->getComponent()->OnEvent(event);
            return true;
        }
        else return false;
    });
    menu |= ui::CatchEvent([&](ui::Event event) {
        if(event == ui::Event().Return){
            std::string execCommand = menuData.visibleApplications[menuData.selectedEntry]->getExecCommand();
            std::system((execCommand+"&").c_str());
            std::cout << "Exec: " << execCommand << std::endl;
            screen.Exit();
            return true;
        }
        return false;
    });
    return menu;
}
