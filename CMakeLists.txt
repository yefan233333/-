cmake_minimum_required(VERSION 3.10)

project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_BUILD_TYPE Debug)
 
find_package(OpenCV REQUIRED)

include_directories(include)

#定义可执行文件的目标文件
# add_executable("${projectName}" main.cpp)

add_executable("${PROJECT_NAME}" main.cpp)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

target_sources(${PROJECT_NAME} PUBLIC ${SRC_FILES})

target_include_directories("${PROJECT_NAME}" PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries("${PROJECT_NAME}" PUBLIC ${OpenCV_LIBS} )
