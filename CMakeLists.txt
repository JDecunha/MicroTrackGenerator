#CMakeLists file for MicroTrackGenerator
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(MicroTrackGenerator VERSION 1.0.0)

###
### Configure Geant4 libraries
###

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#include the Geant4 CMakeLists
include(${Geant4_USE_FILE})

#search for other potential CMakeLists
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})

###
### Configure ROOT libraries
###

find_package(ROOT 6.24 CONFIG REQUIRED)

#include the ROOT CMakelists which defines $ROOT_LIBRARIES
include("${ROOT_USE_FILE}")

###
### Link MicroTrackGenerator with libraries
###

#Identify src and header files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#add our executable, which consists of main file, source files, and headers
add_executable(MicroTrackGenerator MicroTrackGenerator.cc ${sources} ${headers})

#Link executable with Geant and ROOT Libraries
target_link_libraries(MicroTrackGenerator ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS MicroTrackGenerator DESTINATION bin)

