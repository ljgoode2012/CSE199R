cmake_minimum_required(VERSION 3.14)
project(demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt specific CMake features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt directory (adjust path to your Qt installation)
set(Qt6_DIR "C:/Qt/6.10.1/msvc2022_64/lib/cmake/Qt6")

# Set OpenCV directory
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build")

# Find Qt (Widgets module for GUI)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Create executable
# add_executable(demo demo.cpp)



# Link Qt and OpenCV libraries





qt_standard_project_setup()

qt_add_executable(demo
    # mainwindow.ui
    # mainwindow.cpp
    demo.cpp
)

# Include OpenCV directories
target_include_directories(demo PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(demo PRIVATE 
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# set_target_properties(demo PROPERTIES
#     VS_DEBUGGER_ENVIRONMENT "PATH=C:/Users/ljgoo/BYUI-Lindsey/CSE199R/opencv/build/x64/vc16/bin;$ENV{PATH}")

set_target_properties(demo PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=C:/Qt/6.10.1/msvc2022_64/bin;C:/Users/ljgoo/BYUI-Lindsey/CSE199R/opencv/build/x64/vc16/bin;$ENV{PATH}" )