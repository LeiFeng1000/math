add_executable(test_numerical_table main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_numerical_table gtest gtest_main)
