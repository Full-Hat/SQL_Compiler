cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 20)

option(USE_CLANG_C "clang compiler" ON)
option(USE_GCC_C "gcc compiler" OFF)

if (USE_CLANG_C)
    find_program(CLANG "clang++")
    if (CLANG)
        message(STATUS "Use Clang compiler: ${CLANG}")
        set(CMAKE_CXX_COMPILER clang++)
        set(CMAKE_C_COMPILER clang)
    else ()
        message(FATAL_ERROR "Can't find Clang on your pc, try to choose another compiler mannualy or check your installation path")
    endif (CLANG)
endif (USE_CLANG_C)

if (USE_GCC_C)
    find_program(GCC "gcc")
    if (GCC)
        message(STATUS "Use GCC compiler: ${GCC}")
        set(CMAKE_CXX_COMPILER "${GCC}")
        set(CMAKE_C_COMPILER "${GCC}")
    else ()
        message(FATAL_ERROR "Can't find GCC on your pc, try to choose another compiler mannualy or check your installation path")
    endif (GCC)
endif (USE_GCC_C)

option(MAIN "compiler main file" ON)
option(TEST_PARSER "test parser" ON)
option(TEST_UTILS "test utils" ON)

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

set(GTEST_LIBS GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
if(TEST_PARSER)
    add_executable(${project_name}_test_parser test/parser/test.cpp ${SOURCE})
    add_test(test_parser ${project_name}_test_parser)
    target_link_libraries(${project_name}_test_parser PRIVATE ${GTEST_LIBS})
endif(TEST_PARSER)

if(TEST_UTILS)
    add_executable(${project_name}_test_utils Utils/Containers/test.cpp ${SOURCE})
    add_test(test_utils ${project_name}_test_utils)
    target_link_libraries(${project_name}_test_utils PRIVATE ${GTEST_LIBS})
endif(TEST_UTILS)
