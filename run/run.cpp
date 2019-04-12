
#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Game in SDL";





class game
{
private:
    int scrollingOffset = 0;
    int mWidth = 0;
    int mHeight = 0;
    float Position = 880.0f;
	float Velocity = 0.0f;
	float Acceleration = 0.0f;
	float fGravity = 100.0f;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* texture1;

    SDL_Rect catus_rect;

    SDL_Rect dino_rect;
    float fElapsedTime = 0.016f;
    bool isRunning;

    public:
    SDL_Texture* loadTexture( string path );

    bool createImage( SDL_Texture* texture, int x, int y);

    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


    void render(SDL_Window* window, SDL_Renderer* renderer, const SDL_Rect& dino_rect)
    {

    SDL_SetRenderDrawColor(renderer,255 , 0, 255, 255);   // blue
    SDL_RenderClear(renderer);
    render( scrollingOffset, 0 );


    render( scrollingOffset + mWidth, 0 );

    SDL_RenderCopy( renderer, texture1, NULL, &dino_rect );
    SDL_RenderDrawLine(renderer,0, SCREEN_HEIGHT/2 + 160, SCREEN_WIDTH, SCREEN_HEIGHT/2 + 160);

    //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(renderer);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //SDL_UpdateWindowSurface(window);
}

    void create()
    {
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    texture = loadTexture("PHOTO/bg.png");
    texture1 = loadTexture("PHOTO/dino.jpg");
    dino_rect.x = SCREEN_WIDTH / 10;
    dino_rect.y = catus_rect.y = 0;
    dino_rect.w = catus_rect.w = 80;
    dino_rect.h = catus_rect.h = 80;

    catus_rect.x  = 800;


    }
    bool update()
    {
    SDL_Event e;
    while (1) {
        Acceleration += fGravity * fElapsedTime;
        Velocity += Acceleration * fElapsedTime;
        Position += Velocity * fElapsedTime;
        if(Position  > SCREEN_HEIGHT / 2 + 80) Position = SCREEN_HEIGHT / 2 + 80;
        dino_rect.y = Position;

        SDL_PollEvent(&e) ;
        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_KEYDOWN && dino_rect.y == SCREEN_HEIGHT / 2 + 80) {
        	Acceleration = 0.0f;
            Velocity = -fGravity ;

        }



        --scrollingOffset;
				if( scrollingOffset < - mWidth )
				{
					scrollingOffset = 0;
				}

        render(window, renderer, dino_rect);
    }
    return true;
    }

    void free(SDL_Texture* mTexture)
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}


    void quit()
    {
        free(texture);
        quitSDL(window, renderer);
    }
};

void render(SDL_Window* window, SDL_Renderer* renderer, const SDL_Rect& dino_rect);

int main(int argc, char* argv[])
{
    // Your code here
    game run;
    run.create();

    run.update();

    run.quit();



    return 0;
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
            mWidth = 800;
			mHeight = 600;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}


bool game::createImage( SDL_Texture* texture, int x, int y)
{
    if( texture == NULL ) return false;
    catus_rect.x = x;
    catus_rect.y = y;

    SDL_RenderCopy( renderer, texture, &dino_rect, catus_rect );
    return true;
}

void game::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, angle, center, flip );

}



