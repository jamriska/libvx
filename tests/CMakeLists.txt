
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(createandteardown createandteardown.c)

target_link_libraries(createandteardown vx)
