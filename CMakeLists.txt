cmake_minimum_required (VERSION 3.8)

project(boxer CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS " -pthread -ggdb" )

link_libraries(stdc++fs)
include_directories(
	.
	src
	/usr/include
	/usr/local/include
	include
)

link_directories(
	lib		
)

# find_package(zlib)

OPTION(DEFINE_DEBUG "Add -DDEBUG option" OFF)
IF(DEFINE_DEBUG)
	ADD_DEFINITIONS(-DDEBUG)
ENDIF(DEFINE_DEBUG)

FILE(GLOB_RECURSE SOURCES
        src/*.h
        src/*.cpp
        )

add_executable(boxer
	${LIBRARY_FILES}
	${SOURCES}	
	)

# Find a zlib library.
find_library(ZLIB_LIBRARY_PATH NAMES libz.so PATHS ${PROJECT_SOURCE_DIR}/lib NO_DEFAULT_PATH)
add_library(CUSTOM_ZLIB_LIBRARY SHARED IMPORTED)
set_property(TARGET CUSTOM_ZLIB_LIBRARY PROPERTY IMPORTED_LOCATION ${ZLIB_LIBRARY_PATH})

message("Project Source Dir " ${PROJECT_SOURCE_DIR})
message("zlib location " ${ZLIB_LIBRARY_PATH})
message("zlib: " ${ZLIB_LIBRARY})

# Link libraries.
target_link_libraries(boxer pthread CUSTOM_ZLIB_LIBRARY)

get_target_property(OUT boxer LINK_LIBRARIES)
message(STATUS ${OUT} )
