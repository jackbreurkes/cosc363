cmake_minimum_required(VERSION 2.8)

project(assignment)

add_executable(assignment.out assignment.cpp)

#set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8")
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( assignment.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
