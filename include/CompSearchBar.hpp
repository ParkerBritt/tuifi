#ifndef COMP_SEARCH_BAR_HPP
#define COMP_SEARCH_BAR_HPP

#include <ftxui/component/component.hpp>
#include <vector>

#include "Application.hpp"

namespace ui = ftxui;

class SearchBar : public ui::ComponentBase {
public:
    // constructors
    SearchBar();
    virtual ~SearchBar() = default;

    // overrides
    bool Focusable() const override;
    ui::Element Render() override;
    bool isFocusable = false;
    ui::Component& getComponent();
    ui::Component input_;
    void setupSearchEvent(std::vector<std::shared_ptr<launcher::Application>>& applications, std::vector<std::shared_ptr<launcher::Application>>& menuApplications, std::vector<std::string>& menuEntries);

private:
    ui::InputOption inputOptions_;
    std::string inputStr_ = "";
};


#endif
