
include_directories(${CMAKE_SOURCE_DIR}/include)

if(WIN32)

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)

		message (STATUS "Compiling for 64-bit Windows")

		add_definitions(-DWIN64)
	else()

		message (STATUS "Compiling for 32-bit Windows")

		add_definitions(-DWIN32)
	endif()

	# suppress implicit-link file
	add_definitions(-DSS_WIN_DISKUTIL_h_implicit_link)
endif(WIN32)


add_subdirectory(scratch)


# ############################## end of file ############################# #

