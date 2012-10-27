
find_package(GStreamer0.10)

set(srcs
	backend.c
	kernel.c
	context.c
	source.c
#	frame.c
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
	
if(GSTREAMER_FOUND)
	include_directories(${GSTREAMER_INCLUDE_DIRS})
	list(APPEND backend_srcs
		_backends/gst010/gst010_backend.c
		_backends/gst010/gst010_backend.h
		)
	list(APPEND backend_libs ${GSTREAMER_LIBRARIES})
endif()

set(VX_LIBRARY_TARGET)
option(VX_BUILD_STANDALONE_LIBRARY off)
if(VX_BUILD_STANDALONE_LIBRARY)
	set(VX_LIBRARY_TARGET SHARED)
else()
	set(VX_LIBRARY_TARGET STATIC)
endif()

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
