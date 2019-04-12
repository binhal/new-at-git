#include <iostream>
#include <SDL.h>
#include<SDL_image.h>



class GameObject {


public:
    GameObject(const char* texturesheet, SDL_Renderer* ren);
    //~GameObject();

    void Update();
    void Render();

private:

    int xpos;
    int ypos:

    SDL_Texture* objTexture;
    SDL_Rect srcRect, desRect;
    SDL_Renderer* renderer;

};

