
find_package(GStreamer0.10)

set(srcs
	kernel.c
	context.c
	source.c
	)

file(GLOB internal_hdrs
	"_*.h"
	)

set(misc
	config.h.cmake
	${CMAKE_SOURCE_DIR}/AUTHORS
	${CMAKE_SOURCE_DIR}/README
	${CMAKE_SOURCE_DIR}/LICENSE
	)

include_directories(
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
	)

file(GLOB hdrs "${CMAKE_SOURCE_DIR}/include/vx/*.h")

set(backend_srcs
	_backends/null/null_backend.c
	_backends/null/null_backend.h
	_backends/v4l2/v4l2_backend.c
	_backends/v4l2/v4l2_backend.h
	)

set(backend_libs)

find_path(AVFOUNDATION_INCLUDE_DIR AVFoundation/AVFoundation.h)

if(AVFOUNDATION_INCLUDE_DIR)

	message(STATUS "AVFoundation: YES")

	set(HAVE_AVFOUNDATION 1)

	list(APPEND backend_srcs
		_backends/avfoundation/avfoundation_backend.m
		_backends/avfoundation/avfoundation_backend.h
		)

	find_library(AVFOUNDATION_LIBRARY AVFoundation)
	find_library(COREMEDIA_LIBRARY CoreMedia)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	find_library(FOUNDATION_LIBRARY Foundation)

	list(APPEND backend_libs ${AVFOUNDATION_LIBRARY} ${FOUNDATION_LIBRARY} ${COREMEDIA_LIBRARY} ${COREVIDEO_LIBRARY})

else()
	message(STATUS "AVFoundation: NO")
endif()

find_path(QTKIT_INCLUDE_DIR QTKit/QTkit.h)

if(QTKIT_INCLUDE_DIR)
	set(HAVE_QTKIT 1)
	message(STATUS "QTkit: YES")
	find_library(QTkit QTKIT_LIBRARY)

	list(APPEND backend_srcs
		_backends/qtkit/qtkit_backend.m
		_backends/qtkit/qtkit_backend.h
		)

	find_library(COREMEDIA_LIBRARY CoreMedia)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	find_library(QTKIT_LIBRARY QTkit)

	list(APPEND backend_libs  ${COREMEDIA_LIBRARY} ${COREVIDEO_LIBRARY} ${QTKIT_LIBRARY})

else()
	message(STATUS "QTkit: NO")

endif()

if(GSTREAMER_FOUND)

	message(STATUS "GStreamer: YES")

	include_directories(${GSTREAMER_INCLUDE_DIRS})
	set(HAVE_GSTREAMER 1)
	list(APPEND backend_srcs
		_backends/gst010/gst010_backend.c
		_backends/gst010/gst010_backend.h
		)
	list(APPEND backend_libs ${GSTREAMER_LIBRARIES})
else()
	message(STATUS "GStreamer: NO")
endif()

set(VX_LIBRARY_TARGET)
option(VX_BUILD_STANDALONE_LIBRARY off)
if(VX_BUILD_STANDALONE_LIBRARY)
	set(VX_LIBRARY_TARGET SHARED)
else()
	set(VX_LIBRARY_TARGET STATIC)
endif()



configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
	${CMAKE_BINARY_DIR}/include/vx/config.h
	)

add_library(vx ${VX_LIBRARY_TARGET} ${srcs} ${hdrs} ${internal_hdrs} ${backend_srcs} ${CMAKE_BINARY_DIR}/include/vx/config.h)

target_link_libraries(vx ${backend_libs})

if(VX_BUILD_STANDALONE_LIBRARY)
	set_property(TARGET vx
		APPEND PROPERTY
		COMPILE_DEFINITIONS
		VX_BUILD_LIBRARY
		)
else()
	set_property(TARGET vx
		APPEND PROPERTY
		COMPILE_DEFINITIONS
		VX_LIBRARY_STATIC
		)
endif()
