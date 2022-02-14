# Clip Library
# Copyright (c) 2015-2021 David Capello

cmake_minimum_required(VERSION 3.1.2)

project(clip)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  # Use libc++ explicitly so we can compile for
  # CMAKE_OSX_DEPLOYMENT_TARGET=10.7 or 10.8
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


set(CLIP_SOURCES clip.cpp image.cpp)

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  list(APPEND CLIP_SOURCES clip_win.cpp)
elseif(APPLE)
  add_definitions(-fobjc-arc)

  find_library(COCOA_LIBRARY Cocoa)
  if(COCOA_LIBRARY)
    list(APPEND CLIP_SOURCES clip_osx.mm)
  else()
    list(APPEND CLIP_SOURCES clip_none.cpp)
  endif()
elseif(UNIX)
  include(CheckIncludeFiles)
  check_include_files(xcb/xcb.h HAVE_XCB_XLIB_H)

  if(HAVE_XCB_XLIB_H)
    add_definitions(-DHAVE_XCB_XLIB_H)
    list(APPEND CLIP_SOURCES clip_x11.cpp)
  else()
    list(APPEND CLIP_SOURCES clip_none.cpp)
  endif()
else()
  list(APPEND CLIP_SOURCES clip_none.cpp)
endif()

add_library(clip OBJECT ${CLIP_SOURCES})
set_property(TARGET clip PROPERTY POSITION_INDEPENDENT_CODE ON)

if(WIN32)
  target_link_libraries(clip shlwapi)

  # MinGW requires the windowscodecs just because CLSIDs are defined
  # in the windowscodecs.a file instead of the wincodec.h file (?!)
  if(MINGW)
    find_library(CLIP_WINDOWSCODECS_LIBRARY windowscodecs)
    if(CLIP_WINDOWSCODECS_LIBRARY)
      target_link_libraries(clip ${CLIP_WINDOWSCODECS_LIBRARY})
    endif()
  endif()
elseif(APPLE)
  if(COCOA_LIBRARY)
    target_link_libraries(clip ${COCOA_LIBRARY})
  endif()
elseif(UNIX)
  if(HAVE_XCB_XLIB_H)
    target_link_libraries(clip xcb pthread)
  endif()
endif()