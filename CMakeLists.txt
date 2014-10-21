#Copyright 2014 Julius Ikkala
#
#This file is part of GLSLTest.
#
#GLSLTest is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#GLSLTest is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with GLSLTest.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
project(GLSLTest C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

option(GENERATE_DOCS "Generate documentation" ON)
option(DEBUG "Compile in debug mode" ON)

if(DEBUG)
    add_definitions(-g3 -Og)
else()
    add_definitions(-DNDEBUG -O3)
endif()

if(GENERATE_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(
            "${PROJECT_SOURCE_DIR}/doc/Doxyfile.in"
            "${PROJECT_SOURCE_DIR}/doc/Doxyfile"
            @ONLY
        )
        add_custom_target(
            documentation
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc/Doxyfile
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating documentation..." VERBATIM
        )
    else()
        message(SEND_ERROR "Doxygen not found, but doc generation is ON")
    endif()
endif()

add_definitions(-Wall -pedantic -std=c89)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${GLEW_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

file(GLOB SRC_C "${PROJECT_SOURCE_DIR}/src/*.c")
add_executable(glsltest ${SRC_C})
target_link_libraries(glsltest ${GLEW_LIBRARY} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

install(TARGETS glsltest DESTINATION bin)
