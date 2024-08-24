cmake_minimum_required (VERSION 3.11)
# set up project
project(tui_app_launcher LANGUAGES CXX VERSION 1.0.0)

# cmake options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
 
# fetch ftxui
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# add boost
find_package(Boost 1.81 REQUIRED COMPONENTS filesystem)
# set(Boost_INCLUDE_DIR /usr/include)
# set(Boost_LIBRARY_DIR /usr/lib64)

# get source files
file(GLOB SOURCES src/*.cpp)

# get header files
include_directories(${Boost_INCLUDE_DIR} include)
 
# set bin dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE Boost::filesystem
  ${Boost_LIBRARIES}
)

# debug
message(STATUS "boost libraries: " ${Boost_LIBRARIES})
