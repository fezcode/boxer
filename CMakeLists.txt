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

# Find a tar library.
# find_library(TAR_LIBRARY_PATH NAMES libchilkat-9.5.0.a PATHS ${PROJECT_SOURCE_DIR}/lib NO_DEFAULT_PATH)
# add_library(TAR_LIB STATIC IMPORTED)
# set_property(TARGET TAR_LIB PROPERTY IMPORTED_LOCATION ${TAR_LIBRARY_PATH})

# message("Project Source Dir " ${PROJECT_SOURCE_DIR})
# message("Tar lib location " ${TAR_LIBRARY_PATH})

# Link libraries.
target_link_libraries(boxer pthread)
