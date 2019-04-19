#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<sstream>
#include<SDL_mixer.h>
#include"game.h"
using namespace std;

extern const int SCREEN_WIDTH = 800;
extern const int SCREEN_HEIGHT = 600;
extern const char WINDOW_TITLE[] = "Game in SDL";


int main(int argc, char* argv[])
{
    game run;
    run.create();
    run.update();
    run.quit();
    return 0;
}

