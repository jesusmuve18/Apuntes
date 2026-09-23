cmake_minimum_required(VERSION 3.10)

PROJECT(mh_p1 CXX)

file(GLOB C_SOURCES "src/*.cpp")
include_directories("common" "inc")

add_executable(main "main.cpp" ${C_SOURCES})

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# file(GLOB TEST "test/*.cpp")
# add_executable(all_test ${TEST})
# target_link_libraries(all_test GTest::gtest_main)

# gtest_discover_tests(all_test)
