
#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<sstream>
#include<chrono>
#include<time.h>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Game in SDL";
TTF_Font *gFont = NULL;

string int_to_string(int n)
{
    stringstream s ;
    s << n;
    return s.str();
}
class game
{
private:
    int num = 0;
    string text = "Score: ";
    string hi = "Hi: ";
    int high_score = 0;
    int score = 0;
    int speed = 5;
    int scrollingOffset = 0;
    int scrollingOffset2 = 0;
    int mWidth = 0;
    int mHeight = 0;
    int Width = 0;
    int Height = 0;
    float Position = 880.0f;
	float Velocity = 0.0f;
	float Acceleration = 0.0f;
	float fGravity = 100.0f;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* texture1;
    SDL_Texture* texture2;
    SDL_Texture* texture3;
    SDL_Texture* texture4;
    SDL_Texture* text_texture;
    SDL_Texture* text_texture2;
    SDL_Rect catus_rect;
    SDL_Event e;
    SDL_Rect dino_rect;
    float fElapsedTime = 0.06f;
    bool isRunning = false;
    bool hasCollided = false;

    public:
    SDL_Texture* loadFromRenderedText( std::string textureText, SDL_Color textColor );

    SDL_Texture* loadTexture( string path );

    bool createImage( SDL_Texture* texture, int x, int y);

    void render( int x, int y, int w, int h,SDL_Texture* texture, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


    void render(SDL_Window* window, SDL_Renderer* renderer, const SDL_Rect& dino_rect)
    {

    SDL_SetRenderDrawColor(renderer,0 , 0, 0,  0xFF );   // blue
    SDL_RenderClear(renderer);
    //SDL_RenderCopy( renderer, texture2, NULL, &catus_rect );

    render( scrollingOffset, 0 ,mWidth, mHeight,texture);
    render( scrollingOffset + mWidth, 0  , mWidth, mHeight,texture);
    render( 600,200  ,Width + 20, Height,text_texture);
    render( 600,250  ,Width, Height,text_texture2);
    SDL_RenderCopy( renderer, texture2, NULL, &catus_rect );
if(isRunning == false ) SDL_RenderCopy( renderer, texture3, NULL, NULL );
if(hasCollided == true) SDL_RenderCopy( renderer, texture4, NULL, NULL );
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
    gFont = TTF_OpenFont( "arial.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    texture = loadTexture("PHOTO/bg.png");
    texture1 = loadTexture("PHOTO/dino.jpg");
    texture2 = loadTexture("PHOTO/catus.png");
    texture3 = loadTexture("PHOTO/bg_start.png");
    texture4 = loadTexture("PHOTO/bg_reset.png");
    text_texture = loadFromRenderedText(text, {0,0,0,0xFF});
    text_texture2 = loadFromRenderedText(hi, {0,0,0,0xFF});
    dino_rect.x = SCREEN_WIDTH / 10;
    dino_rect.y =  SCREEN_HEIGHT / 2 + 80;
    dino_rect.w =  80;
    dino_rect.h  = 80;
    catus_rect.h = 50;
    catus_rect.y = SCREEN_HEIGHT / 2 + 80 + 80 - catus_rect.h ;
    catus_rect.w = 40;

    catus_rect.x  = scrollingOffset + SCREEN_WIDTH;


    }

    bool update()
    {
    while (1) {
            if(hasCollided == true ){
                    score = 0;
                 scrollingOffset = 0;
                 scrollingOffset2 = 0;
                 num = 0;
                dino_rect.y =  SCREEN_HEIGHT / 2 + 80;
                render(window, renderer, dino_rect);
                SDL_WaitEvent(&e);
                if (e.type == SDL_QUIT) break;
            if(e.type == SDL_KEYDOWN ) {isRunning = true;
            hasCollided = false;}
            }

    if(isRunning == false) {
            render(window, renderer, dino_rect);
            SDL_WaitEvent(&e);
            if (e.type == SDL_QUIT) break;
            if(e.type == SDL_KEYDOWN ) isRunning = true;
    }
    else{
        Acceleration += fGravity * fElapsedTime;
        Velocity += Acceleration * fElapsedTime;
        Position += Velocity * fElapsedTime;
        if(Position  > SCREEN_HEIGHT / 2 + 80) Position = SCREEN_HEIGHT / 2 + 80;
        dino_rect.y = Position;

        SDL_PollEvent(&e) ;
        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_KEYDOWN && dino_rect.y == SCREEN_HEIGHT / 2 + 80 && e.key.keysym.sym == SDLK_UP) {
        	Acceleration = 0.0f;
            Velocity = -fGravity ;

        }



        scrollingOffset = scrollingOffset - speed;
        scrollingOffset2 = scrollingOffset2 - speed;
        catus_rect.x = scrollingOffset2 + SCREEN_WIDTH + num;
				if( scrollingOffset < - mWidth - 1)
				{
					scrollingOffset = 0;
				}
				if(catus_rect.x < 0){ num = rand() % 800 + 1;
				scrollingOffset2 = 0;
				catus_rect.x = scrollingOffset2 + SCREEN_WIDTH + num;}
        if(dino_rect.x + dino_rect.w  >= catus_rect.x && dino_rect.y >= catus_rect.y - 50 && dino_rect.x - 10 <= catus_rect.x + catus_rect.w) hasCollided = true;
        else{
            if(dino_rect.x == catus_rect.x && dino_rect.y <= catus_rect.y - 50) score++;

        cout << num << endl;
        }
        if(score > high_score) high_score = score;
        text_texture = loadFromRenderedText(text + int_to_string(score), {0,0,0,0xFF});
        text_texture2 = loadFromRenderedText(hi + int_to_string(high_score),{0,0,0,0xFF});
        render(window, renderer, dino_rect);
    }
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
        free(texture1);
        free(texture2);
        free(texture3);
        free(texture4);
        free(text_texture);
free(text_texture2);
        quitSDL(window, renderer);
    }
};

void render(SDL_Window* window, SDL_Renderer* renderer, const SDL_Rect& dino_rect);

int main(int argc, char* argv[])
{
    // Your code here
    srand(time(NULL));
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

    SDL_RenderCopy( renderer, texture, &dino_rect, &catus_rect );
    return true;
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
            Width = textSurface->w;
            Height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture;
}

