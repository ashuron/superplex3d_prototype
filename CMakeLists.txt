
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

IF(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
ENDIF()

INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckIncludeFile)

CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)
IF(HAVE_EXECINFO_H)
	ADD_DEFINITIONS(-DHAVE_EXECINFO_H)
ENDIF()

CHECK_C_SOURCE_COMPILES("
	#include <stdio.h>
	int main() {
		char buf[256];
		snprintf(buf, sizeof(buf), \"%d\", 4);
		return 0;
	}
	"
	HAVE_SNPRINTF
)

CHECK_C_SOURCE_COMPILES("
	#include <stdio.h>
	int main() {
		char buf[256];
		_snprintf(buf, sizeof(buf), \"%d\", 4);
		return 0;
	}
	"
	HAVE__SNPRINTF
)

IF(HAVE_SNPRINTF)
	ADD_DEFINITIONS(-DHAVE_SNPRINTF)
ENDIF()

IF(HAVE__SNPRINTF)
	ADD_DEFINITIONS(-DHAVE__SNPRINTF)
ENDIF()

ADD_SUBDIRECTORY(engine)

INCLUDE(engine/cmake/Common.cmake)
INCLUDE(engine/cmake/GeneratePackedData.cmake)

GENERATE_PACKED_DATA(PACKED_FILE
	shaders/terminal.ps
	sounds/boom.wav
	sounds/eat_base.wav
	sounds/eat_infotron.wav
	sounds/music.ogg
	sounds/zonk_shift.wav
	sounds/zonk_fall.wav
	sounds/bug.wav
	sounds/level_done.wav
	boom.jpg
	levels.dat
	normal1.png
	normal2.png
	terminal.jpg
	terminal_n.png
	texture1.jpg
	texture2.jpg
	base.obj
	base.mtl
	exit.obj
	exit.mtl
	hardware1.obj
	hardware1.mtl
	hardware2.obj
	hardware2.mtl
	hardware3.obj
	hardware3.mtl
	hardware4.obj
	hardware4.mtl
	hardware5.obj
	hardware5.mtl
	hardware6.obj
	hardware6.mtl
	hardware7.obj
	hardware7.mtl
	hardware8.obj
	hardware8.mtl
	hardware9.obj
	hardware9.mtl
	infotron_01.obj
	infotron_01.mtl
	infotron_02.obj
	infotron_02.mtl
	infotron_03.obj
	infotron_03.mtl
	infotron_04.obj
	infotron_04.mtl
	infotron_05.obj
	infotron_05.mtl
	infotron_06.obj
	infotron_06.mtl
	infotron_07.obj
	infotron_07.mtl
	infotron_08.obj
	infotron_08.mtl
	infotron_09.obj
	infotron_09.mtl
	infotron_10.obj
	infotron_10.mtl
	infotron_11.obj
	infotron_11.mtl
	infotron_12.obj
	infotron_12.mtl
	infotron_13.obj
	infotron_13.mtl
	infotron_14.obj
	infotron_14.mtl
	infotron_15.obj
	infotron_15.mtl
	infotron_16.obj
	infotron_16.mtl
	infotron_17.obj
	infotron_17.mtl
	infotron_18.obj
	infotron_18.mtl
	infotron_19.obj
	infotron_19.mtl
	infotron_20.obj
	infotron_20.mtl
	infotron_21.obj
	infotron_21.mtl
	infotron_22.obj
	infotron_22.mtl
	infotron_23.obj
	infotron_23.mtl
	infotron_24.obj
	infotron_24.mtl
	infotron_25.obj
	infotron_25.mtl
	infotron_26.obj
	infotron_26.mtl
	infotron_27.obj
	infotron_27.mtl
	infotron_28.obj
	infotron_28.mtl
	infotron_29.obj
	infotron_29.mtl
	murphy_01.obj
	murphy_01.mtl
	murphy_02.obj
	murphy_02.mtl
	murphy_03.obj
	murphy_03.mtl
	murphy_04.obj
	murphy_04.mtl
	murphy_05.obj
	murphy_05.mtl
	murphy_06.obj
	murphy_06.mtl
	murphy_07.obj
	murphy_07.mtl
	murphy_08.obj
	murphy_08.mtl
	murphy_09.obj
	murphy_09.mtl
	murphy_putdisk.obj
	murphy_putdisk.mtl
	murphy_shift.obj
	murphy_shift.mtl
	orange_disk.obj
	orange_disk.mtl
	port_cross.obj
	port_cross.mtl
	port_down.obj
	port_down.mtl
	port_horizontal.obj
	port_horizontal.mtl
	port_left.obj
	port_left.mtl
	port_right.obj
	port_right.mtl
	port_up.obj
	port_up.mtl
	port_vertical.obj
	port_vertical.mtl
	pyramid.obj
	pyramid.mtl
	ram_chip_bottom.obj
	ram_chip_bottom.mtl
	ram_chip_square.obj
	ram_chip_square.mtl
	ram_chip_top.obj
	ram_chip_top.mtl
	red_disk.obj
	red_disk.mtl
	sniksnak_01.obj
	sniksnak_01.mtl
	sniksnak_02.obj
	sniksnak_02.mtl
	sniksnak_03.obj
	sniksnak_03.mtl
	sniksnak_04.obj
	sniksnak_04.mtl
	sniksnak_05.obj
	sniksnak_05.mtl
	sniksnak_06.obj
	sniksnak_06.mtl
	sniksnak_07.obj
	sniksnak_07.mtl
	sniksnak_08.obj
	sniksnak_08.mtl
	sniksnak_09.obj
	sniksnak_09.mtl
	star.obj
	star.mtl
	terminal_back.obj
	terminal_back.mtl
	terminal_front.obj
	terminal_front.mtl
	yellow_disk.obj
	yellow_disk.mtl
	zonk.obj
	zonk.mtl
)

ADD_EXECUTABLE(superplex3d WIN32
	supaplex_def.h
	supaplex_game.cpp
	supaplex_game.h
	superplex_game.cpp
	superplex_game.h
	${PACKED_FILE}
)

TARGET_LINK_LIBRARIES(superplex3d pg_casual pg_sound)
SET_TARGET_PROPERTIES(superplex3d PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
