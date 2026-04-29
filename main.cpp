#include<iostream>
#include<SDL3/SDL.h>

#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>
#include <memory>
#include "bunny.hpp"
#include "geGL/DebugMessage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

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
  ge::gl::setHighDebugMessage();

  auto vsSrc = R".(
  #version 450

  uniform float iTime = 0;
  uniform vec2 pos = vec2(0);
  uniform vec2 pos2 = vec2(0);

  layout(location=0)in vec3 position;
  layout(location=1)in vec3 color   ;

  uniform mat4 model = mat4(1);
  uniform mat4 view  = mat4(1);
  uniform mat4 proj  = mat4(1);

  out vec3 vColor;
  out vec3 vPosition;

  void main(){
    mat4 mvp = proj * view * model;
    gl_Position = mvp*vec4(position,1);
    vColor = color;
    vPosition = position;
  }
  ).";

  auto fsSrc = R".(
  #version 450

  in vec3 vColor;
  in vec3 vPosition;
  out vec4 fColor;

  layout(binding=0)uniform sampler2D myTex;

  void main(){
    fColor = vec4(vColor,1);

    fColor = texture(myTex,vPosition.xz);
  }
  ).";

  auto vs =createShader(GL_VERTEX_SHADER,vsSrc);
  auto fs =createShader(GL_FRAGMENT_SHADER,fsSrc);

  auto prg = glCreateProgram();
  glAttachShader(prg,vs);
  glAttachShader(prg,fs);
  glLinkProgram(prg);


  GLuint modelLoc = glGetUniformLocation(prg,"model");
  GLuint viewLoc  = glGetUniformLocation(prg,"view" );
  GLuint projLoc  = glGetUniformLocation(prg,"proj" );
  
  GLint status;
  glGetProgramiv(prg,GL_LINK_STATUS,&status);
  if(status!=GL_TRUE){
    char buf[10000];
    glGetProgramInfoLog(prg,10000,0,buf);
    std::cerr << buf << std::endl;
  }
 
  float vertices[] = {
    0,0,0,1,0,0,
    1,0,0,0,1,0,
    0,1,0,0,0,1,
    1,1,0,0,0,1,
  };

  uint32_t indices[] = {
    0,1,2,
    2,1,3,
  };

  GLuint vbo;
  glCreateBuffers(1,&vbo);
  glNamedBufferData(vbo,sizeof(bunnyVertices),bunnyVertices,GL_DYNAMIC_COPY);

  GLuint ebo;
  glCreateBuffers(1,&ebo);
  glNamedBufferData(ebo,sizeof(bunnyIndices),bunnyIndices,GL_DYNAMIC_COPY);


  float iTime = 0.f;
  float pos[2] = {0.};
  float pos2[2] = {0.3,0.7};
  bool running = true;

  GLuint vao;
  glCreateVertexArrays(1,&vao);

  glEnableVertexArrayAttrib(vao,0);
  glVertexArrayVertexBuffer(vao,0,vbo,sizeof(float)*0,sizeof(float)*6);
  glVertexArrayAttribBinding(vao,0,0);
  glVertexArrayAttribFormat(vao,0,3,GL_FLOAT,GL_FALSE,0);

  glEnableVertexArrayAttrib(vao,1);
  glVertexArrayVertexBuffer(vao,1,vbo,sizeof(float)*3,sizeof(float)*6);
  glVertexArrayAttribBinding(vao,1,1);
  glVertexArrayAttribFormat(vao,1,3,GL_FLOAT,GL_FALSE,0);

  glVertexArrayElementBuffer(vbo,ebo);


  int x,y,n;
  unsigned char *data = stbi_load("../img.jpg", &x, &y, &n, 0);

  GLuint myTex;
  glCreateTextures(GL_TEXTURE_2D,1,&myTex);
  glTextureStorage2D(myTex,1,GL_RGB8,x,y);
  glTextureSubImage2D(myTex,0,0,0,x,y,GL_RGB,GL_UNSIGNED_BYTE,data);
  glTextureParameteri(myTex,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTextureParameteri(myTex,GL_TEXTURE_MAG_FILTER,GL_NEAREST);


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

    auto model = glm::rotate(glm::mat4(1.f),iTime,glm::vec3(0.f,1.f,0.f));
    auto view  = glm::lookAt(glm::vec3(1.f,1.f,1.f),glm::vec3(0.f),glm::vec3(0.f,1.f,0.f));
    auto proj  = glm::perspective(glm::half_pi<float>(),1024/768.f,0.1f,1000.f);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(prg);

    glProgramUniformMatrix4fv(prg,modelLoc,1,GL_FALSE,(float*)&model);
    glProgramUniformMatrix4fv(prg,viewLoc ,1,GL_FALSE,(float*)&view);
    glProgramUniformMatrix4fv(prg,projLoc ,1,GL_FALSE,(float*)&proj);

    glBindVertexArray(vao);

    glBindTextureUnit(0,myTex);
    glDrawElements(GL_TRIANGLES,sizeof(bunnyIndices)/sizeof(VertexIndex),GL_UNSIGNED_INT,0);
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
