#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<sstream>
#include"game.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Game in SDL";


string game::int_to_string(int n)
{
    stringstream s ;
    s << n;
    return s.str();
}

SDL_Texture* game::loadTexture( string path )
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
            back_ground_width = 800;
			back_ground_height = 600;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}


void game::render( int x, int y, int w, int h, SDL_Texture* texture, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, angle, center, flip );

}
SDL_Texture* game::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{

SDL_Texture* mTexture = NULL;
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels

        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            text_width = textSurface->w;
            text_height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture;
}

void game::draw(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer,0 , 0, 0,  0xFF );   // black
    SDL_RenderClear(renderer);
    // back ground
    render( scrollingOffset, 0 ,back_ground_width, back_ground_height,bg_texture);
    render( scrollingOffset + back_ground_width, 0  , back_ground_width, back_ground_height,bg_texture);
    // score
    render( 600,200  ,text_width + 20, text_height,text_texture);
    render( 600,250  ,text_width, text_height,text_texture2);
    // obstacle
    SDL_RenderCopy( renderer, catus_texture, NULL, &catus_rect );
    if(isRunning == false ) SDL_RenderCopy( renderer, bg_starting_tex, NULL, NULL );
    if(hasCollided == true) SDL_RenderCopy( renderer, bg_reset_tex, NULL, NULL );
    SDL_RenderCopy( renderer, dino_texture, NULL, &dino_rect );
    SDL_RenderDrawLine(renderer,0, SCREEN_HEIGHT/2 + 160, SCREEN_WIDTH, SCREEN_HEIGHT/2 + 160);

    //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(renderer);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //SDL_UpdateWindowSurface(window);
}

void game::create()
{
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    gFont = TTF_OpenFont( "font/arial.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    bg_texture = loadTexture("PHOTO/bg.png");
    dino_texture = loadTexture("PHOTO/dino.jpg");
    catus_texture = loadTexture("PHOTO/catus.png");

    bg_starting_tex = loadTexture("PHOTO/bg_start.png");
    bg_reset_tex = loadTexture("PHOTO/bg_reset.png");

    text_texture = loadFromRenderedText(text, {0,0,0,0xFF});
    text_texture2 = loadFromRenderedText(hi, {0,0,0,0xFF});

    jump = Mix_LoadWAV("sfx/beep-video-game_MkDcnK4O.wav");
    if( jump == NULL )
    {
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    dino_rect.x = SCREEN_WIDTH / 10;
    dino_rect.y =  SCREEN_HEIGHT / 2 + 80;
    dino_rect.w =  80;
    dino_rect.h  = 80;
    catus_rect.h = 50;
    catus_rect.y = SCREEN_HEIGHT / 2 + 80 + 80 - catus_rect.h ;
    catus_rect.w = 40;
    catus_rect.x  = scrollingOffset + SCREEN_WIDTH;
}

void game::update()
{
    while (true)
        {

            if(hasCollided == true ){
                score = 0;
                scrollingOffset = 0;
                dino_rect.y =  SCREEN_HEIGHT / 2 + 80;
                draw(window, renderer);
                SDL_WaitEvent(&e);

                if (e.type == SDL_QUIT) break;
            if(e.type == SDL_KEYDOWN ) {isRunning = true;
            hasCollided = false;}
            }

            if(isRunning == false)
                {
                draw(window, renderer);
                SDL_WaitEvent(&e);
                if (e.type == SDL_QUIT) break;
                if(e.type == SDL_KEYDOWN ) isRunning = true;
                }else
                    {
                        num++;
                    Acceleration += fGravity * fElapsedTime;
                    Velocity += Acceleration * fElapsedTime;
                    Position += Velocity * fElapsedTime;
                    if(Position  > SCREEN_HEIGHT / 2 + 80) Position = SCREEN_HEIGHT / 2 + 80;
                    dino_rect.y = Position;

                    SDL_PollEvent(&e) ;
                    if (e.type == SDL_QUIT) break;
                    if (e.type == SDL_KEYDOWN && dino_rect.y == SCREEN_HEIGHT / 2 + 80 && e.key.keysym.sym == SDLK_UP)
                        {
                        Acceleration = 0.0f;
                        Velocity = -fGravity ;
                        Mix_PlayChannel( -1, jump, 0 );
                        }
                    //scrolling back ground
                    scrollingOffset = scrollingOffset - speed;
                    catus_rect.x = scrollingOffset + SCREEN_WIDTH;
                    if( scrollingOffset < - back_ground_width - 1)
                        {
                            scrollingOffset = 0;
                        }
                    if(dino_rect.x + dino_rect.w  >= catus_rect.x
                        && dino_rect.y >= catus_rect.y - 50
                            && dino_rect.x - 10 <= catus_rect.x + catus_rect.w) hasCollided = true;
                        else
                            if(dino_rect.x == catus_rect.x ) score++;
                    if(score > high_score) high_score = score;
                    text_texture = loadFromRenderedText(text + int_to_string(score), {0,0,0,0xFF});
                    text_texture2 = loadFromRenderedText(hi + int_to_string(high_score),{0,0,0,0xFF});
                    draw(window, renderer);
                    }
        }

}

void game::free(SDL_Texture* mTexture)
    {
	//Free texture if it exists
	if( mTexture != NULL )
        {
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		back_ground_width = 0;
		back_ground_height = 0;
        }
    }

    void game::quit()
    {
        Mix_FreeChunk(jump);
        jump = NULL;
        free(bg_texture);
        free(dino_texture);
        free(catus_texture);
        free(bg_starting_tex);
        free(bg_reset_tex);
        free(text_texture);
        free(text_texture2);
        gFont = NULL;
        quitSDL(window, renderer);
    }

