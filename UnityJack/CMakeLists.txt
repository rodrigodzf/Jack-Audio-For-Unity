cmake_minimum_required(VERSION 3.14)

project(UnityJackAudio)

# set verbose compile options
set( CMAKE_VERBOSE_MAKEFILE ON )

# find modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules" ${CMAKE_MODULE_PATH})
include(${CMAKE_SOURCE_DIR}/cmake-modules/utilities.cmake)

find_package(JACK REQUIRED)
if(JACK_FOUND)
    message(STATUS "JACK_INCLUDE_DIRS: ${JACK_INCLUDE_DIR}")
    message(STATUS "JACK_LIBRARIES: ${JACK_LIBRARY}")
    message(STATUS "JACK_VERSION: ${JACK_VERSION}")
endif()


# library
set( LIB_NAME UnityJackAudio )

# create upper case name
string( TOUPPER ${LIB_NAME} LIB_NAME_UC )

add_library(${LIB_NAME}
            MODULE
            Plugin_TestShared.cpp
            TestSharedLib.cpp
            AudioPluginUtil.cpp
            AudioPluginUtil.h
            AudioPluginInterface.h
            PluginList.h)

target_link_options(${LIB_NAME} PUBLIC -rdynamic)
target_include_directories(${LIB_NAME} PUBLIC ${JACK_INCLUDE_DIR})
target_link_libraries(${LIB_NAME} PUBLIC ${JACK_LIBRARY})
# set_target_properties(${LIB_NAME} PROPERTIES BUNDLE TRUE)

# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES MACOSX_BUNDLE TRUE)
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES BUNDLE_EXTENSION "bundle")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES PREFIX "")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES SUFFIX ".dylib")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES SUFFIX ".bundle")