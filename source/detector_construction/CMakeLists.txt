cmake_minimum_required(VERSION 3.15)

# ROOT
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

# Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${Geant4_INCLUDE_DIR})

include_directories(../detector_construction ../physics_list ../action_initialization ../user_interface)
add_library(DetConst STATIC DetectorConstruction.cpp SensitiveDetector.cpp HitsCollection.cpp)
