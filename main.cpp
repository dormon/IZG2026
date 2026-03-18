#include<iostream>
#include<SDL3/SDL.h>

int main(int argc,char*argv[]){

  auto window = SDL_CreateWindow("IZG2026",1024,768,SDL_WINDOW_OPENGL);

  auto context = SDL_GL_CreateContext(window);


  using GLCLEAR      = void(*)(uint32_t);
  using GLCLEARCOLOR = void(*)(float,float,float,float);
  
  auto glClear      = (GLCLEAR     )SDL_GL_GetProcAddress("glClear"     );
  auto glClearColor = (GLCLEARCOLOR)SDL_GL_GetProcAddress("glClearColor");


  bool running = true;
  while(running){ //main loop
    SDL_Event event;
    while(SDL_PollEvent(&event)){ //event loop
      if(event.type == SDL_EVENT_QUIT)running = false;
    }


#define GL_COLOR_BUFFER_BIT			0x00004000
    //render
    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);


    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DestroyContext(context);
  SDL_DestroyWindow(window);

  std::cerr << "Papoušek je ožralý, ale hodný!" << std::endl;
  return 0;
}
