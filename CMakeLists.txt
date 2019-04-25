cmake_minimum_required(VERSION 2.6)
project(label_tool)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(OpenCV 3 REQUIRED)
include_directories( ${OpenCV_DIRS} )

include_directories( ${PROJECT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE sourcefiles "src/*.cpp" )

add_executable(label_tool main.cpp ${sourcefiles})

TARGET_LINK_LIBRARIES(label_tool  ${OpenCV_LIBS})