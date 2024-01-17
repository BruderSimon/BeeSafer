cmake_minimum_required(VERSION 3.5)
project(BeeSafer)

# For Clangd langage server
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Packages 
find_package(OpenGL REQUIRED)

# Check 
if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (Seperate Build and Source direcotries)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
  message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
	-DGLEW_NO_GLU
	
)

# Building the Dependecies
add_subdirectory(dependencies)
if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  #cmake_policy(SET CMP0026 OLD)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(CreateLaunchers)
endif()

include_directories(
  dependencies/glm/
  dependencies/glfw-3.3.8/include/GLFW/
  dependencies/glew-2.2.0/include/
  )

# Create variable ALL_LIBS that contains the Libaries
# glfw is  defined in its CMakeLists.txt
# GLEW_220 is defined by us in the dependencies/CMakeLists.txt
# OPENGL_LIBARY is a System Variable i guess
set(ALL_LIBS
  ${OPENGL_LIBRARY}
  GLEW_220
  glfw
)

# Add Executeable Here:

add_executable(BeeSafer
  playground.cpp
  playground.h
  RenderingObject.cpp
  RenderingObject.h
  common/shader.cpp
  common/shader.hpp
  common/parse_stl.h
  common/parse_stl.cpp
  common/texture.cpp
  common/texture.hpp
  common/objloader.hpp
  common/objloader.cpp
  common/controls.cpp
  common/controls.hpp
  shader/SimpleFragmentshader.fragmentshader
  shader/SimpleVertexshader.vertexshader
)

target_link_libraries(BeeSafer
  ${ALL_LIBS}
)
# For Visual Studio :|
if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  set_target_properties(BeeSafer PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
  create_target_launcher(BeeSafer WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
  SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/shader/.*" )
  SOURCE_GROUP(res REGULAR_EXPRESSION ".*/res/.*" )

  add_custom_command(
    TARGET BeeSafer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/BeeSafer${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/"
  )
else()
 
# To Copy the Executeable in the source directory
# This is nessecary if we have Shaders, that we Include
		 
add_custom_command(TARGET BeeSafer
                   POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy "BeeSafer${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_SOURCE_DIR}/")
endif(${CMAKE_GENERATOR} MATCHES "Visual Studio")
