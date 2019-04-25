#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<sstream>
#include<chrono>
#include<time.h>
#include<SDL_mixer.h>
#include"game.h"
using namespace std;



int main(int argc, char* argv[])
{
    srand(time(NULL));
    game run;
    run.create();
    run.update();
    run.quit();
    return 0;
}

