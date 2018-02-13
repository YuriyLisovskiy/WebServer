cmake_minimum_required(VERSION 3.9)

set(PROJECT_NAME web_server)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS  "-pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(GLOB LIB_FILES "web_server/src/include/*.h" "web_server/src/*/*.cpp" "web_server/app_start.cpp")

file(GLOB TEST_FILES "web_server/test/*.h")

set(LIB_SRC ${LIB_FILES} web_server/demo/demo.h)

add_executable(${PROJECT_NAME} ${LIB_SRC} ${TEST_SRC})
