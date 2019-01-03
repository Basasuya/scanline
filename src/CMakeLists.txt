find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")



add_executable(test main.cpp Obj.cpp Obj.h Point.h Tool.h Scanline.cpp Scanline.h) 
target_link_libraries(test ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})


