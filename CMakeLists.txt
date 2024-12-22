cmake_minimum_required(VERSION 3.5.0)
project(LSMTree VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    lsm_test
  lsm_test.cc
)
target_link_libraries(
    lsm_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests( lsm_test)

