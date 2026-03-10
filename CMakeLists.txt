cmake_minimum_required(VERSION 3.14)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Enable Qt specific CMake features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt directory (adjust path to your Qt installation)
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64")
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64/lib")

# # Set OpenCV directory: prefer ENV{OpenCV_DIR}, then infer from PATH (Program Files), fallback to local ./opencv/build
# if(NOT DEFINED OpenCV_DIR)
#     if(DEFINED ENV{OpenCV_DIR})
#         set(OpenCV_DIR $ENV{OpenCV_DIR})
#     elseif("$ENV{PATH}" MATCHES "[Oo]pen[Cc][Vv]")
#         if(EXISTS "C:/Program Files/opencv/build")
#             set(OpenCV_DIR "C:/Program Files/opencv/build")
#         elseif(EXISTS "C:/Program Files (x86)/opencv/build")
#             set(OpenCV_DIR "C:/Program Files (x86)/opencv/build")
#         else()
#             set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build")
#         endif()
#     else()
#         set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv/build")
#     endif()
# endif()

# Find Qt (Widgets module for GUI)
find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools Multimedia Test)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Fetch rlottie (Lottie JSON renderer)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(LOTTIE_MODULE OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    rlottie
    GIT_REPOSITORY https://github.com/Samsung/rlottie.git
    GIT_TAG v0.2
)
FetchContent_MakeAvailable(rlottie)

# Create executable
# Link Qt and OpenCV libraries

qt_standard_project_setup()

qt_add_executable(app
    resources.qrc
    main.cpp
    Controllers/mainWindow.cpp
    Controllers/mainWindow.h
    Views/mainWindow.ui
    Models/cameraModel.cpp
    Models/cameraModel.h
    Models/FileManager.cpp
)

# Tell CMake where to find includes
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Tell Qt's UIC where to find .ui files
set_target_properties(app PROPERTIES 
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Views"
)

# Include OpenCV directories
target_include_directories(app PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${rlottie_SOURCE_DIR}/inc
)

target_link_libraries(app PRIVATE 
    Qt6::Widgets
    Qt6::UiTools
    Qt6::Multimedia
    rlottie
    # ${OpenCV_LIBS}
)

# set_target_properties(demo PROPERTIES
#     VS_DEBUGGER_ENVIRONMENT "PATH=C:/Users/ljgoo/BYUI-Lindsey/CSE199R/opencv/build/x64/vc16/bin;$ENV{PATH}")

set_target_properties(app PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=C:/Qt/6.10.1/msvc2022_64/bin;C:/Users/ljgoo/BYUI-Lindsey/CSE199R/opencv/build/x64/vc16/bin;$ENV{PATH}" )

# ============================================================================
# Unit Tests
# ============================================================================
enable_testing()

# Test: FileManager
qt_add_executable(test_FileManager
    Tests/test_FileManager.cpp
    Models/FileManager.cpp
    Models/FileManager.h
)

target_include_directories(test_FileManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_FileManager PRIVATE Qt6::Test Qt6::Core)

add_test(NAME FileManagerTests COMMAND test_FileManager)

# Test: CameraModel
qt_add_executable(test_CameraModel
    Tests/test_CameraModel.cpp
    Models/cameraModel.cpp
    Models/cameraModel.h
    IObservers.h
)

target_include_directories(test_CameraModel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test_CameraModel PRIVATE ${rlottie_SOURCE_DIR}/inc)
target_link_libraries(test_CameraModel PRIVATE 
    Qt6::Test 
    Qt6::Core 
    Qt6::Multimedia
    rlottie
)

add_test(NAME CameraModelTests COMMAND test_CameraModel)