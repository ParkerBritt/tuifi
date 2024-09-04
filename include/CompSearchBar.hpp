#ifndef COMP_SEARCH_BAR_HPP
#define COMP_SEARCH_BAR_HPP

#include <ftxui/component/component.hpp>
#include <vector>

#include "Application.hpp"
#include "MenuData.hpp"

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
    void setupSearchEvent(std::vector<std::shared_ptr<launcher::Application>>& applications, launcher::MenuData& menuData);

private:
    ui::InputOption inputOptions_;
    std::string inputStr_ = "";
};


#endif
