cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

option(MAIN "compiler main file" ON)
option(TEST_PARSER "test parser" OFF)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(project_name SQL_Compiler)
project(
    ${project_name}
    VERSION 1.0
    DESCRIPTION "project description"
)

set(SOURCE parser/include/parser.cpp
    Utils/Containers/tree.cpp
    Utils/Containers/vector.cpp)
if(MAIN)
    add_executable(${project_name} main.cpp ${SOURCE})
endif(MAIN)

include_directories(${CMAKE_SOURCE_DIR}/Utils)

# libraries

enable_testing()
find_package(GTest CONFIG REQUIRED)

if(TEST_PARSER)
    add_executable(${project_name}_test_parser test/parser/test.cpp ${SOURCE})
    add_test(test_parser projectName_test)
    target_link_libraries(${project_name}_test_parser PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
endif(TEST_PARSER)

