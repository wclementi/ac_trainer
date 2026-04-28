cmake_minimum_required(VERSION 4.3)

project(ac_trainer VERSION 1.0.0 LANGUAGES CXX)

# Require C++23 as the latest stable C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE integration (fixes "Too many errors emitted")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4-docking
)
FetchContent_MakeAvailable(imgui)

# Define ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)

# Global include directories are omitted in favor of target_include_directories

# Define source files
set(SOURCES
    src/main.cpp
    src/proc.cpp
    src/geom.cpp
    src/overlay.cpp
    src/esp.cpp
    src/aimbot.cpp
    src/game.cpp
    src/effects.cpp
    ${IMGUI_SOURCES}
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for precompiled and standard headers
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# Specify the precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE include/stdafx.h)

# Link required Windows libraries for D3D11 and DWM (Overlay)
target_link_libraries(${PROJECT_NAME} PRIVATE d3d11 d3dcompiler dwmapi)

# Require Administrator privileges on Windows for memory reading/writing
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/MANIFESTUAC:level='requireAdministrator' uiAccess='false'")
endif()
