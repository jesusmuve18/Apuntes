cmake_minimum_required(VERSION 3.0)
PROJECT(cec17)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
ADD_EXECUTABLE(test "test.cc")
ADD_EXECUTABLE(testrandom "testrandom.cc")
ADD_EXECUTABLE(testsolis "testsolis.cc")
ADD_LIBRARY("cec17_test_func" SHARED "cec17_test_func.c" "cec17.c")
TARGET_LINK_LIBRARIES(test "cec17_test_func")
TARGET_LINK_LIBRARIES(testrandom "cec17_test_func")
TARGET_LINK_LIBRARIES(testsolis "cec17_test_func")

# ── Target: SCA ──────────────────────────────────────────────
add_executable(sca sca.cpp)
target_link_libraries(sca cec17_test_func)

# ── Target: SCA-LS ───────────────────────────────────────────
add_executable(sca_ls sca_ls.cpp)
target_link_libraries(sca_ls cec17_test_func)

# ── Target: ASCA ─────────────────────────────────────────────
add_executable(asca asca.cpp)
target_link_libraries(asca cec17_test_func)