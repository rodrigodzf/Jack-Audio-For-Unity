cmake_minimum_required(VERSION 3.5)
PROJECT(UnityJackAudio)
SET(MACOSX_BUNDLE_BUNDLE_NAME UnityJackAudio)
ADD_LIBRARY(UnityJackAudio
            MODULE
            # UnityNativeAudio.cpp
            # UnityNativeAudio.h
            Plugin_TestShared.cpp
            TestSharedLib.cpp
            AudioPluginUtil.cpp
            AudioPluginUtil.h
            AudioPluginInterface.h
            PluginList.h)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../sample_jackscene/Assets/Plugins)


SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
include(${CMAKE_SOURCE_DIR}/cmake-modules/utilities.cmake)
# SET(CMAKE_SHARED_LINKER_FLAGS "-ljack")


FIND_PACKAGE(JACK REQUIRED)
if(JACK_FOUND)
    message(STATUS "JACK_INCLUDE_DIRS: ${JACK_INCLUDE_DIRS}")
    message(STATUS "JACK_LIBRARIES: ${JACK_LIBRARIES}")
    message(STATUS "JACK_VERSION: ${JACK_VERSION}")
endif()
include_directories(${JACK_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(UnityJackAudio ${JACK_LIBRARIES})
set_target_properties(UnityJackAudio PROPERTIES BUNDLE TRUE)

# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES MACOSX_BUNDLE TRUE)
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES BUNDLE_EXTENSION "bundle")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES PREFIX "")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES SUFFIX ".dylib")
# SET_TARGET_PROPERTIES(UnityJackAudio PROPERTIES SUFFIX ".bundle")