#include "CompSearchBar.hpp"
#include <boost/algorithm/string/case_conv.hpp>
#include <ftxui/component/component.hpp>
#include "Application.hpp"
#include "IconMap.hpp"
#include <vector>
#include "Application.hpp"

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
void SearchBar::setupSearchEvent(std::vector<std::shared_ptr<launcher::Application>>& applications, std::vector<std::shared_ptr<launcher::Application>>& menuApplications, std::vector<std::string>& menuEntries){
        input_ |= ui::CatchEvent([&](ui::Event event) {
        // catch typing events
        if(event.is_character() || event == ui::Event::Backspace){

            // clear previous menu entries
            menuEntries.clear();
            menuApplications.clear();

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
                    menuApplications.push_back(application);
                    menuEntries.push_back(application->getDisplayName());
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
