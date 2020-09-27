cmake_minimum_required(VERSION 2.8)
project(ATtiny84-SNES2DB9)

# setup AVR toolchain
include(AVR.cmake)

# the target CPU
set(AVR_MCU attiny84)

# target application
set(target_name SNES2DB9)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/../common)

# the SNES2DB9 core
add_library(SNES2DB9_common
	${PROJECT_SOURCE_DIR}/../common/snes2db9.h
	${PROJECT_SOURCE_DIR}/../common/snes2db9_mapper.c
	${PROJECT_SOURCE_DIR}/../common/snes2db9_reader.c
	${PROJECT_SOURCE_DIR}/../common/snes2db9_setdb9.c
)

# helper command to update version.h, enforce update of timestamp compiled into executable
add_custom_target( update_version_h ALL
 COMMAND cmake -E touch ${PROJECT_SOURCE_DIR}/version.h
)

# the program
add_avr_executable(${target_name}
	main.c
	version.c
	version.h
	attiny84-gpio.c
	attiny84-gpio.h
)
target_link_libraries(${target_name}-${AVR_MCU}.elf SNES2DB9_common)
add_dependencies(${target_name}-${AVR_MCU}.elf update_version_h)
add_definitions(-DF_CPU=1000000)




