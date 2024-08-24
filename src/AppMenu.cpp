#include "AppMenu.hpp"

ui::Component launcher::makeMenu(std::vector<std::string>& menuEntries, std::vector<std::shared_ptr<launcher::Application>>& visibleApplications, int& selectedEntry, std::shared_ptr<SearchBar>& input){
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

    auto menu = ui::Menu(&menuEntries, &selectedEntry, menuOptions);
    menu |= ui::CatchEvent([&](ui::Event event){
        if(event.is_character() || event == ui::Event::Backspace){
            input->getComponent()->OnEvent(event);
            return true;
        }
        else return false;
    });
    menu |= ui::CatchEvent([&](ui::Event event) {
        if(event == ui::Event().Return){
            std::cout << visibleApplications[selectedEntry]->getExecCommand()  << std::endl;
            return true;
        }
        return false;
    });
    return menu;
}
