#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>

#include <vector>
#include "common/parse_stl.h"
#include "common/objloader.hpp"
#include "RenderingObject.h"
#include "glm/fwd.hpp"

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;

//global variables to handle the MVP matrix
GLuint View_Matrix_ID;
glm::mat4 V;
GLuint Projection_Matrix_ID;
glm::mat4 P;
GLuint Model_Matrix_ID;

RenderingObject ground;
RenderingObject bunny;
RenderingObject obj3;

float curr_x;
float curr_y;
float curr_angle;
float fTime;
int fTimeLocation;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float cam_z;
glm::vec3 camPosition;
glm::vec3 lookAtPoint;
glm::vec3 upVector;
glm::vec3 lightPos;

float light_circle_angle;


int main( void ); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeMVPTransformation();
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW

void updataMovingObjectTransformation();


#endif
