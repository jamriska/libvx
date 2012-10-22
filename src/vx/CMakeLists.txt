
set(srcs
	kernel.c
	context.c
	)

set(misc
	config.h.cmake
	)

include_directories(
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/include
	)

file(GLOB hdrs "${CMAKE_SOURCE_DIR}/include/vx/*.h")


add_library(vx SHARED ${srcs} ${hdrs} ${CMAKE_BINARY_DIR}/include/vx/config.h)
