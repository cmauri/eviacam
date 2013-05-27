cmake_minimum_required(VERSION 2.6)
project(simplelog)

set (SIMPLELOG_MAJOR_VERSION 0)
set (SIMPLELOG_RELEASE_VERSION 5)
set (SIMPLELOG_EPOCH_VERSION 0)

SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries as default?")

set(simplelog_HEADERS
	simplelog.h
)

set(simplelog_SRCS
	${simplelog_HEADERS}
	simplelog.c
)

add_library (simplelog ${simplelog_SRCS})
#add_definitions(-DSIMPLELOG_BUILD)

set_target_properties(simplelog PROPERTIES 
	VERSION "${SIMPLELOG_MAJOR_VERSION}.${SIMPLELOG_RELEASE_VERSION}.${SIMPLELOG_EPOCH_VERSION}"
	SOVERSION ${SIMPLELOG_MAJOR_VERSION}
	)
	
install (FILES ${simplelog_HEADERS} DESTINATION "include")
if (NOT LIBRUNTIMEDIR)
	IF(WIN32)
		set (LIBRUNTIMEDIR "bin")
	ELSE(WIN32)
		set (LIBRUNTIMEDIR "lib")
	ENDIF(WIN32)
endif()
INSTALL (TARGETS simplelog RUNTIME DESTINATION ${LIBRUNTIMEDIR} LIBRARY DESTINATION "lib" ARCHIVE DESTINATION "lib")
