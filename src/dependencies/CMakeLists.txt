add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_subdirectory(glfw-3.3.8)

include_directories(
  glfw-3.3.8/include/GLFW/
  glew-2.2.0/include/
)

set(GLEW_SOURCE
  glew-2.2.0/src/glew.c
)

add_library(GLEW_220 STATIC
  ${GLEW_SOURCE}
  ${GLEW_INCLUDE}
)

target_link_libraries(GLEW_220
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)
