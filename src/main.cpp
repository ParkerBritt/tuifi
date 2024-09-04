// ftxui
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

// boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>


// std c++ libs
#include <fstream>
#include <memory>
#include <string>
#include <unordered_set>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>

// custom components
#include "CompSearchBar.hpp"
#include "AppMenu.hpp"
#include "Application.hpp"

namespace ui = ftxui;
namespace bfs = boost::filesystem;

std::vector<bfs::path> getDesktopFiles(std::string dataDirsRaw){
    // split env var into vector
    std::vector<std::string> dataDirsSplit;
    boost::split(dataDirsSplit, dataDirsRaw, boost::is_any_of(":"));

    // filter to only unique data dirs
    std::vector<std::string> uniqueDataDirs;
    std::unordered_set<std::string> seenDataDirs;
    for(auto dataDir : dataDirsSplit){
        if(seenDataDirs.find(dataDir) == seenDataDirs.end()){
            seenDataDirs.insert(dataDir);
            uniqueDataDirs.push_back(dataDir);
        }
    }

    std::vector<bfs::path> desktopFilePaths;
    std::unordered_set<std::string> seenDesktopFiles;
    for(int i=0; i<uniqueDataDirs.size(); i++){
        bfs::path path = bfs::path(uniqueDataDirs[i])/"applications";
        if(!bfs::exists(path)) continue;

        for (bfs::directory_entry& dirEntry : bfs::directory_iterator(path)){
            std::string desktopFilePath = dirEntry.path().filename().string();

            // check file ends with .desktop
            std::string fileExtension = ".desktop";
            bool correctExtension = desktopFilePath.size()>=fileExtension.size() &&
            desktopFilePath.substr(desktopFilePath.size()-fileExtension.size()) == fileExtension;
            if(!correctExtension) continue;

            // check file is unique
            if(seenDesktopFiles.find(desktopFilePath) != seenDesktopFiles.end()) continue;
            seenDesktopFiles.insert(desktopFilePath);
            desktopFilePaths.push_back(dirEntry.path());
        }
    }

    return desktopFilePaths;
}

struct UIParts {
    ftxui::ScreenInteractive screen;
};


int main(){
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();

    // create components
    std::vector<ui::Component> components;

    std::shared_ptr<SearchBar> input = std::make_shared<SearchBar>();

    // check XDG_DATA_DIRS env var is set
    const char* envTemp = std::getenv("XDG_DATA_DIRS");
    if(!envTemp) throw std::runtime_error("XDG_DATA_DIRS not set");

    std::vector<bfs::path> desktopFilePaths = getDesktopFiles(std::string(envTemp));


    std::vector<std::string> appNames;
    std::vector<std::shared_ptr<launcher::Application>> applications;
    for(auto desktopFilePath : desktopFilePaths){
        std::shared_ptr<launcher::Application> app = std::make_shared<launcher::Application>(desktopFilePath);
        applications.push_back(app);
    }
    std::sort(applications.begin(), applications.end(), [](const std::shared_ptr<launcher::Application>& a, const std::shared_ptr<launcher::Application>& b){
        return *a < *b;
    });


    launcher::MenuData menuData;
    for(int i=0; i<10; i++){
        menuData.menuEntries.push_back(applications[i]->getDisplayName());
        menuData.visibleApplications.push_back(applications[i]);
    }
    input->setupSearchEvent(applications, menuData);


    // create menu
    ui::Component menu = launcher::makeMenu(menuData, input, screen);

    ui::Component mainLayout;
    std::string execCommand;
    // on exec command
    menu |= ui::CatchEvent([&](ui::Event event){
        if(event == ui::Event().Return){
            execCommand = menuData.visibleApplications[menuData.selectedEntry]->getExecCommand();
            int execStatus = std::system((execCommand+"&").c_str());

            mainLayout->DetachAllChildren();
            ui::Component message;
            if(execStatus == 0){ // if command successfully executed
                message = ui::Renderer( [&] {
                    return ui::text("Running command: " + execCommand);
                });
            }
            else {
                message = ui::Renderer( [&] {
                    return ui::text("Failed to run command: " + execCommand);
                });
            }
            mainLayout->Add(message | ui::borderRounded);
            screen.Exit();

            return true;
        }
        return false;
    });


    components = {
        input->getComponent(),
        menu,
    };
    mainLayout = ui::Container::Vertical(components);

    menu->TakeFocus();
    screen.Loop(mainLayout);
}
