cmake_minimum_required(VERSION 3.10)

project(AudioPluginJack)

# set verbose compile options
set(CMAKE_VERBOSE_MAKEFILE OFF)

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
set(LIB_NAME AudioPluginJack)

# create upper case name
string(TOUPPER ${LIB_NAME} LIB_NAME_UC)

add_library(
    ${LIB_NAME}
    MODULE
    Plugin.cpp
    JackClient.h
    AudioPluginUtil.cpp
    AudioPluginUtil.h
    AudioPluginInterface.h
    PluginList.h)

target_include_directories(${LIB_NAME} PUBLIC ${JACK_INCLUDE_DIR})
target_link_libraries(${LIB_NAME} PUBLIC ${JACK_LIBRARY})

if(CMAKE_CXX_COMPILER_ID STREQUAL "gcc")
    target_link_options(${LIB_NAME} PUBLIC -rdynamic)
    target_compile_options(${LIB_NAME} PRIVATE -Werror -Wall -Wextra -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/libAudioPluginJack.so"
        "${CMAKE_SOURCE_DIR}/../Packages/com.rodrigodzf.jackaudio/Runtime/Plugins")
endif()

# set_target_properties(${LIB_NAME} PROPERTIES BUNDLE TRUE)

# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES MACOSX_BUNDLE TRUE)
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES BUNDLE_EXTENSION "bundle")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES PREFIX "")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES SUFFIX ".dylib")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES SUFFIX ".bundle")
