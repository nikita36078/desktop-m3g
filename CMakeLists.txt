#
# M3G project config file.
#

# project info
project (M3G)

# required cmake
cmake_minimum_required (VERSION 2.0)

# dependend libraries
find_package (ZLIB   REQUIRED)
find_package (PNG    REQUIRED)
find_package (JPEG   REQUIRED)
find_package (OpenGL REQUIRED)

set (M3G_REQUIRED_LIBRARIES
         ${ZLIB_LIBRARIES}
         ${PNG_LIBRARIES}
         ${JPEG_LIBRARIES}
         ${OPENGL_LIBRARIES})
set (M3G_REQUIRED_INCLUDE_DIR 
         ${ZLIB_INCLUDE_DIR}
         ${PNG_INCLUDE_DIR}
         ${JPEG_INCLUDE_DIR}
         ${OPENGL_INCLUDE_DIR})

# build type = Debug (defualt)
if (NOT CMAKE_BUILD_TYPE)
    message (STATUS "Build type is not specified, then")
    message (STATUS "  set CMAKE_BUILD_TYPE=Debug")
    message (STATUS "  set CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/bin")
    set (CMAKE_BUILD_TYPE "Debug")
    set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)
endif (NOT CMAKE_BUILD_TYPE)
message (STATUS "Configuring for ${CMAKE_BUILD_TYPE}...")


# dependent custom libraries
add_subdirectory (exlib)



# project src
add_subdirectory (src)

