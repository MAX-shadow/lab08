cmake_minimum_required(VERSION 3.10)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.26.9.tar.gz"
    SHA1 "a962c9f0cfd6f8fc62ea25943d89debfa61d4d4a"
    LOCAL
)

project(lab08)

set(BANKING_VERSION_MAJOR 0)
set(BANKING_VERSION_MINOR 1)
set(BANKING_VERSION_PATCH 0)
set(BANKING_VERSION_TWEAK 0)
set(BANKING_VERSION "${BANKING_VERSION_MAJOR}.${BANKING_VERSION_MINOR}.${BANKING_VERSION_PATCH}.${BANKING_VERSION_TWEAK}")
set(BANKING_VERSION_STRING "v${BANKING_VERSION}")

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(banking)

add_executable(demo demo/main.cpp)
target_link_libraries(demo banking)
install(TARGETS demo RUNTIME DESTINATION bin)

if(BUILD_TESTS)
    enable_testing()

    hunter_add_package(GTest)
    find_package(GTest CONFIG REQUIRED)

    add_executable(check tests/test_account.cpp tests/test_transaction.cpp)
    target_link_libraries(check banking GTest::gtest_main)
    target_include_directories(check PRIVATE banking)

    add_test(NAME check COMMAND check)
endif()

include(CPackConfig.cmake)
