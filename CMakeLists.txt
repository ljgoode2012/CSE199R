cmake_minimum_required(VERSION 3.14)
project(CSE199R)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenCV directory
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build")

# Find OpenCV
find_package(OpenCV REQUIRED)

# Create executable
add_executable(demo demo.cpp)

# Include OpenCV directories
target_include_directories(demo PRIVATE ${OpenCV_INCLUDE_DIRS})

# Link OpenCV libraries
target_link_libraries(demo PRIVATE ${OpenCV_LIBS})

set_target_properties(demo PROPERTIES
    VS_DEBUGGER_ENVIRONMENT "PATH=C:/Users/ljgoo/BYUI-Lindsey/CSE199R/opencv/build/x64/vc16/bin;$ENV{PATH}"
)
