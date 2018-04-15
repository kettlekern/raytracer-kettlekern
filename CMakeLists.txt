cmake_minimum_required(VERSION 3.1)

# Name of the project
project(lab1)
set (CMAKE_CXX_STANDARD 11)

# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp")

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h")



# Set the executable.
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})





# Add GLM
# Get the GLM environment variable. Since GLM is a header-only library, we
# just need to add it to the include directory.
set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if(GLM_INCLUDE_DIR)
  include_directories(${GLM_INCLUDE_DIR})
  message(STATUS "GLM environment variable found")
else()
# If the GLM_INCLUDE_DIR environment variable is not set, we assume
# the user has installed GLM properly on their system
  message(STATUS "GLM environment variable `GLM_INCLUDE_DIR` not found, GLM must be installed with the system")
endif()

option(sceneinfo "../Resources/simple.pov")
