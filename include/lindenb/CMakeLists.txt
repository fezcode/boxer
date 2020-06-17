cmake_minimum_required (VERSION 3.8)

project(lindenb CXX)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS " -pthread " )

FILE(GLOB_RECURSE LIBRARY_FILES
        *.h
        *.cpp
        )

add_executable(lindenb
	${LIBRARY_FILES}
	)
