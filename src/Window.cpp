/**
 * Window implementation
 * @author Simon Engel
 */

#include "Window.hpp"
#include "glfw3.h"
#include <cstdio>

GLFWwindow* window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			   
Window::Window(int width, int height, const char* title, Game* game): m_width{width}, m_height{height}, gamePtr{game}{

}

bool Window::init(){
  glewExperimental = true;
  if(!glfwInit()){
    std::fprintf( stderr, "Failed to initialzie GLFW");
    return false;
  }
 
  glfwWindowHint(GLFW_SAMPLES, 4);
  // We could use newer Version of OpenGL but we want to Support as many devices as possible
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
  
  window = glfwCreateWindow(m_width, m_height, "BeeSafe", NULL, NULL);
  if( window == NULL){
    std::fprintf(stderr, "Failed to open GLFW Window");
    glfwTerminate();
    return false;
  }
  // To place the Window in the Middle
  int xpos, ypos, width, height;
  GLFWmonitor* primary = glfwGetPrimaryMonitor();
  glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
  glfwSetWindowPos(window, (width - m_width)/2, (height - m_height)/2);
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr, "Failed to initialize GLEW\n");
    return false;
  }
  glEnable(GL_DEPTH_TEST);
  //glClearColor(0.686f, 0.886f, 0.91f, 1.0f);
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  // Pointer to the Window Wrapper class
  glfwSetWindowUserPointer(window, this);
  // Key Call Back class
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
 
  return true;
}

void Window::swapBuffer(){
  // Swap buffer
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Window::clear(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window::destroy(){
  glfwSetWindowShouldClose(window, true);
  glfwTerminate();
}
/**
 * https://gamedev.stackexchange.com/questions/58541/how-can-i-associate-a-key-callback-with-a-wrapper-class-instance
 */

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  Window* window_pntr = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if(action == GLFW_PRESS){
    switch(key){
    case GLFW_KEY_ESCAPE:
      // Paus Game or Exit if already Paused
      if(window_pntr->gamePtr->State == Game_Menu)
	window_pntr->gamePtr->State = Game_End;
      else if(window_pntr->gamePtr->State == Game_Active)
	window_pntr->gamePtr->State = Game_Menu;
      break;
    case GLFW_KEY_ENTER:
      /*
      if(window_pntr->gamePtr->State == Game_Menu)
	window_pntr->gamePtr->State = Game_Active;
      // std::fprintf(stderr, "Key Enter");
      */
      break;
    }
  }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
  Window* window_pntr = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    GLdouble xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    //Calculate Normal Display Coordinates
    GLfloat NDC_x = (2 * xpos) / window_pntr->m_width - 1.0f;
    GLfloat NDC_y = -(2 * ypos) / window_pntr->m_height + 1.0f;
    
  }     
}
