/**
 * Window Header file
 * @author Simon Engel
 */
#ifndef GUI_HPP
#define GUI_HPP

#include <GL/glew.h>

#include "game.hpp"

#include <glfw3.h>

#include <cstdio>
#include <stdio.h>

class Window{
public:
  Window(int width, int height, const char* title, Game* game);
  bool init();
  void swapBuffer();
  void destroy();
  void clear();
  bool getESC_pressed(){return esc_pressed;}
  bool esc_pressed = false;
  //void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  int m_width, m_height;
  Game* gamePtr;
};

#endif
