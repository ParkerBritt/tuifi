#include "Application.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include "IconMap.hpp"
#include <boost/algorithm/string.hpp>

namespace bfs = boost::filesystem;
using namespace launcher;

Application::Application(bfs::path desktopPath){
    Application::desktopPath = desktopPath;

    std::string desktopFilePathStr = desktopPath.string();
    std::ifstream desktopFileStream(desktopFilePathStr);

    // read file
    if(!desktopFileStream.is_open()){
        std::cerr << "failed to open file: " << desktopFilePathStr << std::endl; // add file path
    }

    std::string curLine;

    bool nameFound = false;
    bool execFound = false;
    while (getline(desktopFileStream, curLine)){
        if(!nameFound && curLine.substr(0, 5)=="Name="){
            Application::appName = curLine.substr(5);
        }
        else if(!execFound && curLine.substr(0, 5)=="Exec="){
            Application::execCommand = formatExecCommand(curLine.substr(5));
        }
        if(nameFound && execFound) break;
    }

    desktopFileStream.close();

    // set icon
    lowerAppName = appName;
    boost::algorithm::to_lower(lowerAppName);
    // default icon
    Application::icon = "󰘔";
    if(iconNameMap.find(lowerAppName) != iconNameMap.end()){
        Application::icon = iconNameMap[lowerAppName];
    }
    else{
        bool foundKeyword = false;
        for(auto i : iconKeywordMap){
            std::string keyword = i.first;
            std::string icon = i.second;

            // find keyword in app name
            // size_t foundPos = lowerAppName.find(keyword);
            std::vector<std::string> appNameSplit;
            boost::split(appNameSplit, lowerAppName, boost::is_any_of(" "));
            for(std::string namePart : appNameSplit){
                if(keyword == namePart){
                    foundKeyword = true;
                    break;
                };
            }

            // if keyword not in app name found check next keyword
            if(!foundKeyword) continue;

            Application::icon = icon;
            break;
        }
    }
}

// formatters
std::string Application::formatExecCommand(std::string unformattedCommand){
    std::string formattedCommand;
    size_t strLen = unformattedCommand.size();

    size_t start = 0;
    size_t end = 0;
    for(size_t i=0; i<strLen; i++){
        const char c = unformattedCommand[i];
        if(i == strLen-1){
            formattedCommand+=unformattedCommand.substr(start, strLen-start+1);
        }
        else if(c == '%'){
            size_t end = i;
            formattedCommand+=unformattedCommand.substr(start, end-start);

            i+=2;
            size_t start = i;
        }
    }
    return formattedCommand;
}

// getters
std::string Application::getAppName() const{
    return appName;
};
std::string Application::getExecCommand() const{
    return execCommand;
};
std::string Application::getDisplayName() const{
    return icon + " " + Application::getAppName();
}
std::string Application::getLowerAppName() const{
    return lowerAppName;
}
bfs::path Application::getDesktopPath() const{
    return desktopPath;
}

// operators
bool Application::operator<(const Application& rhs) const{
    return appName < rhs.getAppName();
}
