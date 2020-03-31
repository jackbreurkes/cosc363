cmake_minimum_required(VERSION 2.8)

project(lab4)

add_executable(Boat.out Boat.cpp)
add_executable(Vase.out Vase.cpp)
#add_executable(DevIL_Test.out DevIL_Test.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( Boat.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
target_link_libraries( Vase.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
#target_link_libraries( DevIL_Test.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
