cmake_minimum_required(VERSION 3.0)
project(ride-c)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
include_directories("duktape" "src")
#set_source_file_properties("duktape/duktape.c" PROPERTIES COMPILER_FLAGS "-O3 -DNDEBUG")
set(RIDE_C_SRC
    "duktape/duktape.c"
    "src/ride.c"
    "src/ridejs.c")
add_library(ride_c ${RIDE_C_SRC})
target_link_libraries(ride_c m)

add_executable(ridejs_test "test.c")
target_link_libraries(ridejs_test ride_c)
