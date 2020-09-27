#
# AVR gcc CMake Toolchain definition
# based on an example by Natesh Narain (http://nnarain.github.io/2016/03/29/AVR-CMake-Toolchain.html)
#
# toolchain prefix
set(TRIPLE "avr")

# attempt to find avr-gcc
find_path(TOOLCHAIN_ROOT
	NAMES
		${TRIPLE}-gcc

	PATHS
		/usr/bin
		/usr/local/bin
		/bin

		$ENV{AVR_ROOT}
)

# Error, could not find toolchain
if(NOT TOOLCHAIN_ROOT)
	message(FATAL_ERROR "Toolchain root could not be found!!!")
endif(NOT TOOLCHAIN_ROOT)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_CROSSCOMPILING "true")

set(CMAKE_C_COMPILER   "${TOOLCHAIN_ROOT}/${TRIPLE}-gcc${OS_SUFFIX}"     CACHE PATH "gcc"     FORCE)
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/${TRIPLE}-g++${OS_SUFFIX}"     CACHE PATH "g++"     FORCE)
set(CMAKE_AR           "${TOOLCHAIN_ROOT}/${TRIPLE}-ar${OS_SUFFIX}"      CACHE PATH "ar"      FORCE)
set(CMAKE_LINKER       "${TOOLCHAIN_ROOT}/${TRIPLE}-ld${OS_SUFFIX}"      CACHE PATH "linker"  FORCE)
set(CMAKE_NM           "${TOOLCHAIN_ROOT}/${TRIPLE}-nm${OS_SUFFIX}"      CACHE PATH "nm"      FORCE)
set(CMAKE_OBJCOPY      "${TOOLCHAIN_ROOT}/${TRIPLE}-objcopy${OS_SUFFIX}" CACHE PATH "objcopy" FORCE)
set(CMAKE_OBJDUMP      "${TOOLCHAIN_ROOT}/${TRIPLE}-objdump${OS_SUFFIX}" CACHE PATH "objdump" FORCE)
set(CMAKE_STRIP        "${TOOLCHAIN_ROOT}/${TRIPLE}-strip${OS_SUFFIX}"   CACHE PATH "strip"   FORCE)
set(CMAKE_RANLIB       "${TOOLCHAIN_ROOT}/${TRIPLE}-ranlib${OS_SUFFIX}"  CACHE PATH "ranlib"  FORCE)
set(AVR_SIZE           "${TOOLCHAIN_ROOT}/${TRIPLE}-size${OS_SUFFIX}"    CACHE PATH "size"    FORCE)

# disable strange gcc assumptions about rdynamic
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(AVR_LINKER_LIBS "-lc -lm -lgcc")
set(AVR_CFLAGS "-Os -std=c99 -Wall -Wextra -ftrapv -Wunreachable-code -Wswitch-enum -Wswitch-default -Wstrict-prototypes -Wshadow -Wundef -fno-exceptions -ffunction-sections -fdata-sections")

macro(add_avr_executable target_name)

	# define file names we will be using
	set(elf_file ${target_name}-${AVR_MCU}.elf)
	set(map_file ${target_name}-${AVR_MCU}.map)
	set(hex_file ${target_name}-${AVR_MCU}.hex)
	set(lst_file ${target_name}-${AVR_MCU}.lst)


	# add elf target
	add_executable(${elf_file}
		${ARGN}
	)

	# set compile and link flags for elf target
	set_target_properties(
		${elf_file}

		PROPERTIES
			COMPILE_FLAGS "-mmcu=${AVR_MCU} ${AVR_CFLAGS}"
			LINK_FLAGS    "-mmcu=${AVR_MCU} -Wl,-Map,${map_file} ${AVR_LINKER_LIBS}"
	)


	# generate the lst file
	add_custom_command(
		OUTPUT ${lst_file}

		COMMAND
			${CMAKE_OBJDUMP} -h -S ${elf_file} > ${lst_file}

		DEPENDS ${elf_file}		
	)

	# create hex file
	add_custom_command(
		OUTPUT ${hex_file}

		COMMAND
			${CMAKE_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}

		DEPENDS ${elf_file}
	)

	# output section sizes
	add_custom_command(
		OUTPUT "print-size-${elf_file}"

		COMMAND
			${AVR_SIZE} ${elf_file}

		DEPENDS ${elf_file}
	)
	
	# output fuse settings
	add_custom_command(
		OUTPUT "print-fuse-${elf_file}"

		COMMAND
			${CMAKE_OBJDUMP}  -s -j .fuse ${elf_file}

		DEPENDS ${elf_file}
	)

	# build the intel hex file for the device
	add_custom_target(
		${target_name}
		ALL
		DEPENDS ${hex_file} ${lst_file} "print-size-${elf_file}" "print-fuse-${elf_file}"
	)

	set_target_properties(
		${target_name}

		PROPERTIES
			OUTPUT_NAME ${elf_file}
	)

endmacro(add_avr_executable)


