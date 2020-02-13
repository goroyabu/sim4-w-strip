# Project 
project(g4-cdte-dsd CXX)

# C++
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z -Wall")

# Installation
set(CMAKE_INSTALL_PREFIX $ENV{HOME})

# Uninstallation
configure_file(cmake_uninstall.cmake.in cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -P cmake_uninstall.cmake)

# Sub-Directories
add_subdirectory(detector_construction)
add_subdirectory(physics_list)
add_subdirectory(action_initialization)
add_subdirectory(user_interface)
include_directories(detector_construction physics_list action_initialization user_interface)

# ROOT
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

# Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${Geant4_INCLUDE_DIR})

#------------------------------------------------------------

# Build 
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} DetConst PhysList ActInit UsrInt)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

