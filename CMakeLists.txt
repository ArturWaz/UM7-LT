cmake_minimum_required(VERSION 2.8.10)


set(PROJECT_NAME unitTests_um7lt)
project(${PROJECT_NAME})



set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
if ( UNIX )
    set( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
endif ( UNIX )



add_subdirectory(um7lt)


add_executable(${PROJECT_NAME} unitTests/unitTest_um7lt.cpp)
target_link_libraries (${PROJECT_NAME} um7lt boost_unit_test_framework)