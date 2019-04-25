#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<sstream>
#include<chrono>
#include<time.h>
#include"game.h"
#include<vector>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Game in SDL";
const int MAX_OBSTACLE_HEIGHT = 100;


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

void game::draw(SDL_Window* window, SDL_Renderer * ren)
{
    SDL_SetRenderDrawColor(ren,0 , 0, 0,  0xFF );   // black
    SDL_RenderClear(ren);
    // back ground
    render( scrollingOffset, 0 ,back_ground_width, back_ground_height,bg_texture);
    render( scrollingOffset + back_ground_width, 0  , back_ground_width, back_ground_height,bg_texture);
    // score
    render( 600,200  ,text_width + 20, text_height,text_texture);
    render( 600,250  ,text_width, text_height,text_texture2);
    // obstacle
    for(int i = 0; i < catus.size();i++)
    {
        catus[i]->draw(ren,catus_texture);
    }

    // start bg
    if(isRunning == false ) SDL_RenderCopy( ren, bg_starting_tex, NULL, NULL );
    // reset bg
    if(hasCollided == true) SDL_RenderCopy( ren, bg_reset_tex, NULL, NULL );
    // dino animation
    if(dino_rect.y == SCREEN_HEIGHT / 2 + 80){
        SDL_RenderCopy( ren, animation[animation_frame/4], NULL, &dino_rect );
    }else
        SDL_RenderCopy( ren, dino_texture, NULL, &dino_rect );
    SDL_RenderDrawLine(ren,0, SCREEN_HEIGHT/2 + 160, SCREEN_WIDTH, SCREEN_HEIGHT/2 + 160);

    //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(ren);
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

    animation[0] = loadTexture("PHOTO/dino_r.jpg");
    animation[1] = loadTexture("PHOTO/dino_l.jpg");
    animation[2] = animation[0];

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
//    catus_rect.h = 50;
//    catus_rect.y = SCREEN_HEIGHT / 2 + 80 + 80 - catus_rect.h ;
//    catus_rect.w = 40;
//    catus_rect.x  = SCREEN_WIDTH;

}

void game::update()
{
    while (true)
        {

            int frameStart = SDL_GetTicks();
            if(hasCollided == true ){
                for(int i = 0; i < catus.size();i++) delete catus[i];
                catus.erase(catus.begin(),catus.end());
                score = 0;
                speed = 5;
                num = 0;
                rand_num = 100;
                frame = 0;
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

                    Acceleration += fGravity * fElapsedTime;
                    Velocity += Acceleration * fElapsedTime;
                    Position += Velocity * fElapsedTime;
                    if(Position  > SCREEN_HEIGHT / 2 + 80) Position = SCREEN_HEIGHT / 2 + 80;
                    dino_rect.y = Position;
                    if(num == 1000 ) {speed += 1;
                    num = 0;}else num++;


                    SDL_PollEvent(&e) ;
                    if (e.type == SDL_QUIT) break;
                    if (e.type == SDL_KEYDOWN && dino_rect.y == SCREEN_HEIGHT / 2 + 80 && e.key.keysym.sym == SDLK_UP)
                        {
                        Acceleration = 0.0f;
                        Velocity = -fGravity ;
                        num++;
                        Mix_PlayChannel( -1, jump, 0 );
                        }
                    //scrolling back ground
                    scrollingOffset = scrollingOffset - speed;
                    //catus_rect.x = scrollingOffset + SCREEN_WIDTH;
                    if( scrollingOffset < - back_ground_width - 1)
                        {
                            scrollingOffset = 0;
                        }
                    frame++;
                    if(frame ==  rand_num) {
                        catus.push_back(new Obstacle());
                        frame = 0;
                        rand_num = rand() % 500 + 1;

                    }
                    for(int i = 0; i < catus.size();i++)
                    {
                        catus[i]->update(speed);
                        if(catus[i]->get_rect().x <= 0 )
                        {delete catus[i];
                        catus.erase(catus.begin());}
                        hasCollided = if_collided(dino_rect,catus[i]->get_rect());
                    }

                    score++;
                    if(score > high_score) high_score = score;
                    text_texture = loadFromRenderedText(text + int_to_string(score), {0,0,0,0xFF});
                    text_texture2 = loadFromRenderedText(hi + int_to_string(high_score),{0,0,0,0xFF});
                    // cycle animation
                    if(animation_frame/4 >= 2) animation_frame = 0;
                    animation_frame++;
                    draw(window, renderer);

                    int frame = SDL_GetTicks() - frameStart;
                    if(frame < frameDelay )
                    SDL_Delay(frameDelay - frame );

                    }
        }

}

  bool game::if_collided(SDL_Rect a, SDL_Rect obstacle)
  {
      if(a.x + a.w  >= obstacle.x
                        && a.y >= obstacle.y - 50
                            && a.x - 10 <= obstacle.x + obstacle.w) return true;
                            else return false;
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
        for(int i = 0; i < 3; i++){
            free(animation[i]);
        }
        Mix_FreeChunk(jump);
        jump = NULL;
        Mix_FreeChunk(collided);
        collided = NULL;
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

