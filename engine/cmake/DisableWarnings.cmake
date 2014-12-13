
IF(NOT MSVC)
	INCLUDE(CheckCCompilerFlag)
	CHECK_C_COMPILER_FLAG("-w" HAVE_NO_WARNINGS)
ENDIF()

##
## Disables compiler warnings.
##
MACRO(DISABLE_WARNINGS)
	IF(MSVC)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4005 /wd4305")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4005 /wd4305")
	ELSEIF(HAVE_NO_WARNINGS)
		ADD_DEFINITIONS(-w)
	ENDIF()
ENDMACRO()
