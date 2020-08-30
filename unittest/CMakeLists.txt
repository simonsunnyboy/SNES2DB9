#
# SNES to DB9 Joystick converter
#
# (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
# http://www.asmsoftware.de/
#
# The MIT License applies to this software. See COPYING for details.
#
# PC based unittest of common core code
#
cmake_minimum_required(VERSION 2.8)
project(Unittest)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Coverage CACHE STRING
       "Choose the type of build, options are: None Coverage Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    message("gcov enabled")
    add_definitions("-DGCOV_ENABLED")
endif()


# GNU coverage
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
    message("coverage target created...")
    setup_target_for_coverage(test_snes2reader_coverage test_snes2reader coverage)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/code/common
    ${PROJECT_SOURCE_DIR}
)

# the unittest framework library to link with project
add_library(unittest
	${PROJECT_SOURCE_DIR}/framework/unittest.c
	${PROJECT_SOURCE_DIR}/framework/unittest.h
)

# an example test object with implemented unittest
add_executable(test_snes2reader
	${PROJECT_SOURCE_DIR}/code/common/snes2db9.h
	${PROJECT_SOURCE_DIR}/code/common/snes2db9_reader.c
    test_snes2reader.c
)
target_link_libraries(test_snes2reader unittest -lgcov)
