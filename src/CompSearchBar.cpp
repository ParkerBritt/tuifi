#include <boost/algorithm/string/case_conv.hpp>
#include <ftxui/component/component.hpp>
#include "Application.hpp"
#include <vector>
#include "CompSearchBar.hpp"
#include "MenuData.hpp"

namespace ui = ftxui;

SearchBar::SearchBar(){
    // configure input options
    inputOptions_ = ui::InputOption::Spacious();
    inputOptions_.transform = [](ui::InputState state){
        state.element |= ui::borderRounded;
        return state.element;
    };
    inputOptions_.multiline = false;

    input_ = ui::Input(&inputStr_, inputOptions_);


    Add(input_);
}

// --------------
// catch input event and update menu with new search
// -------------
void SearchBar::setupSearchEvent(std::vector<std::shared_ptr<launcher::Application>>& applications, launcher::MenuData& menuData){
        input_ |= ui::CatchEvent([&](ui::Event event) {
        // catch typing events
        if(event.is_character() || event == ui::Event::Backspace){

            // clear previous menu entries
            menuData.menuEntries.clear();
            menuData.visibleApplications.clear();

            // find the new search bar text
            std::string searchValue = inputStr_;
            if(event.is_character()) searchValue += event.character();
            else if(event == ui::Event::Backspace) searchValue = searchValue.substr(0, searchValue.size()-1);

            // iterate through applications
            size_t i = 0;
            for(auto application : applications){
                if(i>10) break;

                // if search input found in name then add to menu
                std::string lowerAppName = application->getLowerAppName();
                size_t foundPos = lowerAppName.find(searchValue);
                if(foundPos != std::string::npos){
                    menuData.visibleApplications.push_back(application);
                    menuData.menuEntries.push_back(application->getDisplayName());
                    i++;
                }
                
            }
        }
        return false;
    });
}

ui::Component& SearchBar::getComponent(){
    return input_;
}

bool SearchBar::Focusable() const {
    return false;
}

ui::Element SearchBar::Render(){
    return input_->Render();
}
