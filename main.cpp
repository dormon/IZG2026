#include<iostream>
#include<SDL3/SDL.h>

#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>
#include <memory>

using namespace ge::gl;

GLuint createShader(GLenum type, std::string src){
  auto vs = glCreateShader(type);
  char const*s[]={
    src.c_str()
  };
  glShaderSource(vs,1,s,0);
  glCompileShader(vs);
  GLint status;
  glGetShaderiv(vs,GL_COMPILE_STATUS,&status);
  if(status != GL_TRUE){
    char buf[10000];
    glGetShaderInfoLog(vs,10000,0,buf);
    std::cerr << buf << std::endl;
  }


  return vs;
}

int main(int argc,char*argv[]){

  auto window = SDL_CreateWindow("IZG2026",1024,768,SDL_WINDOW_OPENGL);

  auto context = SDL_GL_CreateContext(window);

  ge::gl::init();

  auto vsSrc = R".(
  #version 450

  uniform float iTime = 0;
  uniform vec2 pos = vec2(0);
  uniform vec2 pos2 = vec2(0);

  flat out int player;
  void main(){
    if(gl_VertexID==0)gl_Position = vec4(0,0,-1,1)+vec4(pos,0,0);
    if(gl_VertexID==1)gl_Position = vec4(1,0,-1,1)+vec4(pos,0,0);
    if(gl_VertexID==2)gl_Position = vec4(0,1,-1,1)+vec4(pos,0,0);

    if(gl_VertexID==3)gl_Position = vec4(0,0,0,1)+vec4(pos2,0,0);
    if(gl_VertexID==4)gl_Position = vec4(1,0,0,1)+vec4(pos2,0,0);
    if(gl_VertexID==5)gl_Position = vec4(0,1,0,1)+vec4(pos2,0,0);

    player = int(gl_VertexID>=3);
  }
  ).";

  auto fsSrc = R".(
  #version 450

  out vec4 fColor;

  flat in int player;

  void main(){
    if(player==0)fColor = vec4(0,0,1,.5);
    if(player==1)fColor = vec4(1,0,0,.5);
  }
  ).";

  auto vs =createShader(GL_VERTEX_SHADER,vsSrc);
  auto fs =createShader(GL_FRAGMENT_SHADER,fsSrc);

  auto prg = glCreateProgram();
  glAttachShader(prg,vs);
  glAttachShader(prg,fs);
  glLinkProgram(prg);

  
  GLint status;
  glGetProgramiv(prg,GL_LINK_STATUS,&status);
  if(status!=GL_TRUE){
    char buf[10000];
    glGetProgramInfoLog(prg,10000,0,buf);
    std::cerr << buf << std::endl;
  }
    


  float iTime = 0.f;
  float pos[2] = {0.};
  float pos2[2] = {0.3,0.7};
  bool running = true;


  while(running){ //main loop
    SDL_Event event;
    while(SDL_PollEvent(&event)){ //event loop
      if(event.type == SDL_EVENT_QUIT)running = false;
      if(event.type == SDL_EVENT_KEY_DOWN){
        if(event.key.key == SDLK_W)pos[1] += 0.1;
        if(event.key.key == SDLK_S)pos[1] -= 0.1;
        if(event.key.key == SDLK_A)pos[0] -= 0.1;
        if(event.key.key == SDLK_D)pos[0] += 0.1;
        if(event.key.key == SDLK_I)pos2[1] += 0.1;
        if(event.key.key == SDLK_K)pos2[1] -= 0.1;
        if(event.key.key == SDLK_J)pos2[0] -= 0.1;
        if(event.key.key == SDLK_L)pos2[0] += 0.1;
      }
    }


    glClearColor(0.1,0.1,0.1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);


    glUseProgram(prg);
    glDrawArrays(GL_TRIANGLES,0,6);
    glProgramUniform1f(prg,glGetUniformLocation(prg,"iTime"),iTime);
    glProgramUniform2fv(prg,glGetUniformLocation(prg,"pos"),1,pos);
    glProgramUniform2fv(prg,glGetUniformLocation(prg,"pos2"),1,pos2);
    iTime += 0.01f;

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DestroyContext(context);
  SDL_DestroyWindow(window);

  std::cerr << "Papoušek je ožralý, ale hodný!" << std::endl;
  return 0;
}
