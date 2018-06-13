cmake_minimum_required(VERSION 3.9)

set(PROJECT_NAME demo_app)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS  "-pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

file(GLOB LIB_FILES "src/include/*.h" "src/*/*.cpp" "demo_app/*.cpp" "demo_app/*.h")

set(LIB_SRC ${LIB_FILES})

add_executable(${PROJECT_NAME} ${LIB_SRC})
