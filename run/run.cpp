
#include <iostream>
#include <SDL.h>
#include "SDL_utils.h"
#include"texture.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Game in SDL";





class game
{
private:
    float fBirdPosition = 0.0f;
	float fBirdVelocity = 0.0f;
	float fBirdAcceleration = 0.0f;
	float fGravity = 100.0f;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    SDL_Rect filled_rect;
    float fElapsedTime = 0.2f;
    public:
    SDL_Texture* loadTexture( string path );

    bool createImage( SDL_Texture* texture );

    void refreshScreen(SDL_Window* window, SDL_Renderer* renderer, const SDL_Rect& filled_rect)
    {

    // Đặt màu vẽ thành xanh lam (blue), xoá màn hình về màu xanh lam.
    SDL_SetRenderDrawColor(renderer,255 , 0, 255, 255);   // blue
    SDL_RenderClear(renderer);
     createImage(  texture );
    // Đặt màu vẽ về trắng và vẽ hình chữ nhật
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);   // white
    SDL_RenderFillRect(renderer, &filled_rect);
    SDL_RenderDrawLine( renderer, 0, SCREEN_HEIGHT / 2 + 12,SCREEN_WIDTH, SCREEN_HEIGHT / 2 + 12);
    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình


   //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(renderer);
   //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
   //SDL_UpdateWindowSurface(window);
}

    bool create()
    {
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    texture = loadTexture("C:/Users/Binh Fantasia/Downloads/31_scrolling_backgrounds/bg.png");
    filled_rect.x = SCREEN_WIDTH / 6;
    filled_rect.y = SCREEN_HEIGHT / 2;
    filled_rect.w = 16;
    filled_rect.h = 12;

    return true;
    }
    bool update()
    {


    // Định nghĩa toạ độ ban đầu và kích cỡ hình chữ nhật


    // Bước nhảy mỗi khi dịch chuyển
    int step = 4;
    // Xoá toàn bộ màn hình và vẽ
    refreshScreen(window, renderer, filled_rect);
    SDL_Event e;
    while (1) {
        // Đợi 10 mili giây
        //SDL_Delay(10);
        fBirdVelocity += fBirdAcceleration * fElapsedTime;
			fBirdPosition += fBirdVelocity * fElapsedTime;
			if(fBirdPosition  > SCREEN_HEIGHT / 2) fBirdPosition = SCREEN_HEIGHT / 2;
            filled_rect.y = fBirdPosition;

             // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        if (e.type == SDL_QUIT) break;

        // Nếu không có sự kiện gì thì tiếp tục trở về đầu vòng lặp
        if ( SDL_PollEvent(&e) == 0) continue;



        // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
        if (e.type == SDL_KEYDOWN ) {
                //&& e.key.keysym.sym == SDLK_UP
        	fBirdAcceleration = 0.0f;
				fBirdVelocity = -fGravity / 4.0f;
				fBirdPosition -= 100.0f;
        }
        else
            fBirdAcceleration += fGravity * fElapsedTime;

            fBirdVelocity += fBirdAcceleration * fElapsedTime;
			fBirdPosition += fBirdVelocity * fElapsedTime;
			if(fBirdPosition  > SCREEN_HEIGHT / 2) fBirdPosition = SCREEN_HEIGHT / 2;
            filled_rect.y = fBirdPosition;

            // Xoá toàn bộ màn hình và vẽ lại
            refreshScreen(window, renderer, filled_rect);


    }
    return true;
    }
    bool quit()
    {
        SDL_Texture* texture = NULL;
        SDL_DestroyTexture(texture);
        quitSDL(window, renderer);
    }
};

void refreshScreen(SDL_Window* window, SDL_Renderer* renderer, const SDL_Rect& filled_rect);

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
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}


bool game::createImage( SDL_Texture* texture )
{
    if( texture == NULL ) return false;
    SDL_RenderCopy( renderer, texture, NULL, NULL );
    return true;
}








