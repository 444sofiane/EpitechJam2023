cmake_minimum_required(VERSION 3.10)
project(nerdSimulator)

set(CMAKE_CXX_STANDARD 20)

# Add the SFML library
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
include_directories("include")
file(GLOB_RECURSE header_files "src/*.hpp")
foreach(_headerFile ${header_files})
    get_filename_component(_dir ${_headerFile} DIRECTORY)
    include_directories(${_dir})
endforeach()

# Add the source files
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Create the executable
add_executable(nerdSimulator ${SOURCE_FILES})

# Link the SFML library
target_link_libraries(nerdSimulator PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
