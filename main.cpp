#include<iostream>
#include<SDL3/SDL.h>

int main(int argc,char*argv[]){

  SDL_CreateWindow("IZG2026",1024,768,0);
  for(;;);

  std::cerr << "Papoušek je ožralý, ale hodný!" << std::endl;
  return 0;
}
