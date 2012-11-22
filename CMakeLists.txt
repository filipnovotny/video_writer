cmake_minimum_required(VERSION 2.8)

PROJECT( video_writer )

FIND_PACKAGE(VISP REQUIRED)
IF(VISP_FOUND)
  INCLUDE(${VISP_USE_FILE})
ENDIF(VISP_FOUND)

find_package( Boost REQUIRED)
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

add_subdirectory(cmd_line)

ADD_EXECUTABLE( video_writer main.cpp )
TARGET_LINK_LIBRARIES( video_writer boost_program_options cmd_line )