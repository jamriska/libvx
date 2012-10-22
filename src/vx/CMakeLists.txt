
set(srcs
	backend.c
	kernel.c
	context.c
	source.c
	frame.c
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
	_backends/v4l2/v4l2_backend.c
	_backends/v4l2/v4l2_backend.h
	)

add_library(vx SHARED ${srcs} ${hdrs} ${internal_hdrs} ${backend_srcs} ${CMAKE_BINARY_DIR}/include/vx/config.h)
