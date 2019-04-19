#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<sstream>
using namespace std;



class game
{
private:
    int num = 0;
    TTF_Font *gFont = NULL;
    string text = "Score: ";
    string hi = "Hi: ";
    int high_score = 0;
    int score = 0;
    int speed = 5;
    int scrollingOffset = 0;
    int back_ground_width = 0;
    int back_ground_height = 0;
    int text_width = 0;
    int text_height = 0;
    float Position = 880.0f;
	float Velocity = 0.0f;
	float Acceleration = 0.0f;
	float fGravity = 100.0f;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* bg_texture;
    SDL_Texture* dino_texture;
    SDL_Texture* catus_texture;
    SDL_Texture* bg_starting_tex;
    SDL_Texture* bg_reset_tex;
    SDL_Texture* text_texture;
    SDL_Texture* text_texture2;
    SDL_Rect catus_rect;
    SDL_Event e;
    SDL_Rect dino_rect;
    float fElapsedTime = 0.06f;
    bool isRunning = false;
    bool hasCollided = false;
    Mix_Chunk *jump = NULL;

    public:

    SDL_Texture* loadFromRenderedText( std::string textureText, SDL_Color textColor );

    SDL_Texture* loadTexture( string path );

    void render( int x, int y, int w, int h,SDL_Texture* texture, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    void draw(SDL_Window* window, SDL_Renderer* renderer);

    void create();

    void update();

    void free(SDL_Texture* mTexture);

    string int_to_string(int n);

    void quit();
};

