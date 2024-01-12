/**
 * Game class to define all Game Mechanics
 */
#include "game.hpp"
#include <cstdio>
#include <cstdlib>
#include <random>

GLfloat RandomFloat(GLfloat min, GLfloat max);

void Game::init(){
  this->State = Game_Menu;
  score = 0;
  Game::programID = LoadShaders("shader/SimpleVertexshader.vertexshader", "shader/SimpleFragmentshader.fragmentshader");
  bee = RenderingObject();
  bee.InitializeVAO();
  bee.LoadSTL("res/Bee.stl");
}

void Game::update(GLfloat currentTime){
  if(this->State == Game_Active){
    //Not A Number
    if(lastTime == NAN)
      lastTime = currentTime;
    
  }
}

void Game::render(){
  glUseProgram(programID);
  if(this->State == Game_Active){ 
    bee.DrawObject();
  }else if(this->State == Game_Menu || this->State == Game_Over){
   
  }
}

void Game::end(){
  this->State = Game_Over;
}


GLfloat RandomFloat(GLfloat min, GLfloat max){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<GLfloat> dis(min, max);

  
  return dis(gen);
}
