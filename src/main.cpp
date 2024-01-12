/**
 * Staring Point
 * @author Simon Engel
 */

#include "Window.hpp"
#include <cstdio>
#include <thread>
#include <chrono>

int main(){
  Game scene;
  Window window (1920, 1080, "BeeSafer", &scene);
  window.init();
  scene.init();

  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;
  
  while(scene.State != Game_End){
     // calculate delta time
    // --------------------
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    if(deltaTime < 15){
      long sleepTime = 15 - deltaTime;
      std::this_thread::sleep_for(std::chrono::nanoseconds(sleepTime * 1000000));
      //std::printf("Sleep for: %long", sleepTime);

      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
    }
      
      lastFrame = currentFrame;
    
      window.clear();
    
      scene.update(currentFrame);
    
      scene.render();

      window.swapBuffer();
    //std::fprintf(stderr, "Game Loop");
  }
  //Clean UP
  window.destroy();
  scene.end();
  
  return 0;
}
