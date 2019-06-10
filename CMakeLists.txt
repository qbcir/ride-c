cmake_minimum_required(VERSION 3.0)
project(ride-c)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
include_directories("duktape" "src")
#set_source_file_properties("duktape/duktape.c" PROPERTIES COMPILER_FLAGS "-O3 -DNDEBUG")
set(RIDE_C_SRC
    "duktape/duktape.c"
    "src/ride.c"
    "src/ridejs.c")

install(FILES src/ride.h DESTINATION include/ride_c)

add_library(ride_c STATIC ${RIDE_C_SRC})
target_link_libraries(ride_c m)

add_library(ride_c_shared SHARED ${RIDE_C_SRC})
set_target_properties(ride_c_shared PROPERTIES OUTPUT_NAME ride_c)
target_link_libraries(ride_c_shared m)

install(TARGETS ride_c ride_c_shared
        ARCHIVE DESTINATION lib/ride_c
        LIBRARY DESTINATION lib/ride_c
        PUBLIC_HEADER DESTINATION include/ride_c
)

#add_executable(ridejs_test "test.c")
#target_link_libraries(ridejs_test ride_c)

SET(CPACK_SET_DESTDIR "on")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "ride-c-dev")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "C library for working with Ride compiler")
set(CPACK_DEBIAN_FILE_NAME "${CPACK_DEBIAN_PACKAGE_NAME}.${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_amd64.deb")
set(CPACK_PACKAGE_DESCRIPTION "C library for working with Ride compiler")
set(CPACK_PACKAGE_CONTACT "Alexander Golenev alex.golenev@gmail.com")

include(CPack)
