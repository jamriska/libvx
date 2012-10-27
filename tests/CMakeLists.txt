
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(createandteardown createandteardown.c)
target_link_libraries(createandteardown vx)


find_package(SDL)
if(SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
	add_executable(simplesdl simplesdl.c)
	target_link_libraries(simplesdl vx ${SDL_LIBRARY})
endif()
