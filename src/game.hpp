#ifndef GAME_H
#define GAME_H

#include "common/shader.hpp"
#include <vector>
#include "RenderingObject.h"
enum GameState{
  Game_Menu,
  Game_Active,
  Game_Over,
  Game_End
};

class Game{
public:
  GameState State;
  void init();
  void update(GLfloat currentTime);
  void end();
  void render();
  GLboolean processInput = false;
  GLuint programID;
  GLfloat mouse_x, mouse_y;

private:
  GLfloat g_vertex_buffer_data[12];
  GLuint vertexbuffer;
  GLuint VertexArrayID;
  GLfloat lastTime;
  GLuint score;
  RenderingObject bee;
};
#endif
