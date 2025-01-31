cmake_minimum_required(VERSION 3.5)

cmake_path(GET CMAKE_SOURCE_DIR FILENAME PROJECT_ID)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_ID})

project(${PROJECT_NAME}
        VERSION 1.0.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH "../bin")

option(FTXUI_QUIET "" ON)
option(FTXUI_BUILD_EXAMPLES OFF)

include_directories(deps/json)
include_directories(src/utils)
include_directories(src/cli)
include_directories(src/manager)

add_subdirectory(deps/ftxui)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/manager/manager.cpp
        src/cli/cli.cpp
        src/utils/utils.h
)

target_link_libraries(${PROJECT_NAME} 
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)