cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(project_name SQL_Compiler)
project(
    ${project_name}
    VERSION 1.0
    DESCRIPTION "project description"
)

set(SOURCE )
add_executable(${project_name} main.cpp ${SOURCE})

# libraries
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${project_name} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

enable_testing()

add_executable(projectName_test test/parser/test.cpp ${SOURCE})
add_test(AllTestsInMain projectName_test)
