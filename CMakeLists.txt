#
#
# Martin Johansson, 2014
#

cmake_minimum_required (VERSION 2.8)

project (Turmite)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)


# FIND GLUT
#find_package(GLUT REQUIRED)
#include_directories(${GLUT_INCLUDE_DIRS} )
#link_directories(${GLUT_LIBRARY_DIRS})
#add_definitions(${GLUT_DEFINITIONS})
#if(NOT GLUT_FOUND)
#    message(ERROR " GLUT not found!")
#endif(NOT GLUT_FOUND)


# SET paths
SET(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include)
SET(GLEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib)
SET(DevIL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/devIL)

message(STATUS "CMake build tool: " ${CMAKE_BUILD_TOOL})

# ---------------------------------------------------
#

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(DevIL REQUIRED)


link_directories(${OpenGL_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/glut-3.7.6-bin/)

add_definitions(${GLEW_DEFINITIONS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#if(OPENGL_FOUND)
message(STATUS "Detected OpenGL path is: ${OpenGL_LIBRARIES}")
#endif(OPENGL_FOUND)

if(NOT DevIL_FOUND)
	message(ERROR " DEVIL not found! ")
endif(NOT DevIL_FOUND)
message(STATUS "Found DevIL includes at: ${DevIL_INCLUDE_DIR}")

include_directories( 	
	${OpenGL_INCLUDE_DIRS} 
	${GLEW_INCLUDE_DIRS} 
	${DevIL_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/libs/glut-3.7.6-bin/ 
)


# ---------------------------------------------------
# -
#

#if(NOT CMAKE_BUILD_TYPE)
#  set (CMAKE_BUILD_TYPE Release)
#endif(NOT CMAKE_BUILD_TYPE)

# Warning level
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(/W1)
endif()


# ---------------------------------------------------
# Include Files
#


file(GLOB_RECURSE CORE_SOURCES 
src/*.cpp
src/*.h)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(Turmite ${CORE_SOURCES})


# ---------------------------------------------------
# Linking
# 

target_link_libraries(Turmite ${OPENGL_LIBRARIES} ${DevIL_LIBRARIES} glut32 glew32)



