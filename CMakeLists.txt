cmake_minimum_required(VERSION 3.9)
project(WebServer)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS  "-pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(GLOB LIB_FILES
		"WebServer/src/include/*.h"
		"WebServer/src/*/*.cpp")

file(GLOB TEST_FILES
		"WebServer/test/*/*.h"
		"WebServer/test/*/*.cpp")

set(LIB_SRC ${LIB_FILES})


set(TEST_SRC ${TEST_FILES})


add_executable(WebServer ${LIB_SRC} ${TEST_SRC})
