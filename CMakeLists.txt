cmake_minimum_required(VERSION 3.0)

project(math)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB hed_files "src/*.h" "src/*.hpp")
file(GLOB src_files "src/*.cpp")

add_library(math ${hed_files} ${src_files})

install(TARGETS math DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${hed_files} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

add_subdirectory(test)
