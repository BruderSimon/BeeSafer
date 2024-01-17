#include "playground.h"

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
// Include GLM
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  fTime = 0.0;
  
  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("shader/SimpleVertexShader.vertexshader",
			  "shader/FragmentShader.frag");
  //shader/SimpleFragmentShader.fragmentshader
  // shader/FragmentShader.frag

  initializeMVPTransformation();
  fTimeLocation = glGetUniformLocation(programID, "fTime");
  std::cout << fTimeLocation << std::endl;
  curr_x = 0;
  curr_y = 0;
  cam_z = 100;

  camPosition = glm::vec3(0, 0, 10);
  lookAtPoint = glm::vec3(0, 0, 0);

  //start animation loop until escape key is pressed
  do{

    updateAnimationLoop();

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );
	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
  closeWindow();
 
  return 0;
}

void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  if (glfwGetKey(window, GLFW_KEY_W)) curr_y+=0.01;
  else if (glfwGetKey(window, GLFW_KEY_S)) curr_y-=0.1;
  else if (glfwGetKey(window, GLFW_KEY_A)) curr_x-=0.1;
  else if (glfwGetKey(window, GLFW_KEY_D)) curr_x+=0.1;
  else if (glfwGetKey(window, GLFW_KEY_O)) cam_z += 0.1;
  else if (glfwGetKey(window, GLFW_KEY_L)) cam_z -= 0.1;
  initializeMVPTransformation();
  GLfloat currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  //std::cout << deltaTime << std::endl;
  if(deltaTime > 0.1){
    lastFrame = currentFrame;
    fTime +=1.0f;
    // std::cout << fTime << std::endl;
  }
  glUniform1f(fTimeLocation, std::sin(fTime));
  
  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform
  updataMovingObjectTransformation();
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(fTime), glm::vec3(0.0f, 1.0f, 0.0f));

  // Apply the rotation matrix to the model matrix
  bunny.M  = rotationMatrix * bunny.M;
  
  glUniformMatrix4fv(View_Matrix_ID, 1, GL_FALSE, &V[0][0]);
  glUniformMatrix4fv(Projection_Matrix_ID, 1, GL_FALSE, &P[0][0]);
  glUniformMatrix4fv(Model_Matrix_ID, 1, GL_FALSE, &bunny.M[0][0]);
  bunny.DrawObject();

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void updataMovingObjectTransformation()
{
  bunny.M = glm::rotate(glm::mat4(1.0f), -20.1f, { 1.0f,0.0f,0.0f });
  bunny.M = glm::translate(bunny.M, { curr_x,curr_y,0.0f });
}

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Example: simple cube", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.1f, 0.3f, 0.0f);
  return true;
}

bool initializeMVPTransformation()
{
  // Get a handle for our "MVP" uniform
  Model_Matrix_ID = glGetUniformLocation(programID, "M");
  Projection_Matrix_ID = glGetUniformLocation(programID, "P");
  View_Matrix_ID = glGetUniformLocation(programID, "V");
    
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);
   
  // Camera matrix
  V = glm::lookAt(
    glm::vec3(0, 0, cam_z), // Camera is at (4,3,-3), in World Space
    glm::vec3(0, 0, 0), // and looks at the origin
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  
  return true;
  
}

bool initializeVertexbuffer()
{  
  //####################### OBJECT: Bee ###################
  bunny = RenderingObject();
  bunny.InitializeVAO();
  bunny.LoadSTL("res/Bee.stl");
 
  return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteVertexArrays(1, &ground.VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}


