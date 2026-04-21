cmake_minimum_required(VERSION 4.3)

project(ac_trainer VERSION 1.0.0 LANGUAGES CXX)

# Require C++23 as the latest stable C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add headers directory to include path
include_directories(include)

# Define source files
set(SOURCES
    src/main.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include headers folder
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify the precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE include/stdafx.h)
