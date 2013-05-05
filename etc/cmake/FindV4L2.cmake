#
# Find Module v4l2 and libv4l2 which are required on Linux for video capture
#
# (c) 2006-2013 Hartmut Seichter
#  

set(V4L2_FOUND FALSE)
set(LIBV4L2_FOUND FALSE)

if(UNIX) 

	# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")


	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")

	find_path(V4L2_INCLUDE_DIR linux/videodev2.h
		$ENV{V4L2_ROOT}/include
		${V4L2_TARGET_ROOT}/include
		/usr/include 
		/usr/src/linux/include
		NO_DEFAULT_PATH
	)

	if(V4L2_INCLUDE_DIR)
		set(V4L2_FOUND TRUE)
	endif()

	mark_as_advanced(
		V4L2_INCLUDE_DIR
	)

	find_path(LIBV4L2_INCLUDE_DIR libv4l2.h
		$ENV{LIBV4L2_ROOT}/include
		${V4L2_TARGET_ROOT}/include
		/usr/include 
		/usr/src/linux/include
		# yikes - on Scratchbox the headers are actually just links and hence CMake bails out
		${V4L2_TARGET_ROOT}/../opt/maemo/usr/include
#		NO_DEFAULT_PATH
		)

	find_library(LIBV4L2_v4l2_LIBRARY NAMES v4l2
		$ENV{LIBV4L2_ROOT}/lib
		${V4L2_TARGET_ROOT}/../opt/maemo/usr/lib
		${V4L2_TARGET_ROOT}/lib
		/usr/lib
		/usr/src/linux/lib
#		NO_DEFAULT_PATH
	)

	find_library(LIBV4L2_v4lconvert_LIBRARY	NAMES v4lconvert
		$ENV{LIBV4L2_ROOT}/lib
		${V4L2_TARGET_ROOT}/../opt/maemo/usr/lib
		${V4L2_TARGET_ROOT}/lib
		/usr/lib 
		/usr/src/linux/lib
#		NO_DEFAULT_PATH
	)



	if (LIBV4L2_INCLUDE_DIR AND LIBV4L2_v4l2_LIBRARY)

		set(LIBV4L2_LIBRARIES
			${LIBV4L2_v4l2_LIBRARY}
			${LIBV4L2_v4lconvert_LIBRARY}
		)
 
		set(LIBV4L2_FOUND TRUE)

	endif()

endif()
