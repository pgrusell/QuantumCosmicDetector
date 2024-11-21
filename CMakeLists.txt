cmake_minimum_required(VERSION 3.13 FATAL_ERROR)  # CMake version check

project(Simulation)                               # create projecjt

# Adding user interface and visualization packages
find_package(Geant4 REQUIRED ui_all vis_all) 

# Adding files from the path Geant4_use_file (bash variable)
include(${Geant4_USE_FILE})

# Compile all the source files (.cc files) in the project folder
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)

# Compile all the header files (.cc files) in the project folder
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

# Set a name for the executable
add_executable(sim sim.cc ${sources} ${headers})

# link labraries for later implementation
target_link_libraries(sim ${Geant4_LIBRARIES})

# project_name DEPENDS executable 
add_custom_target(Simulation DEPENDS sim)
