#include<vector>
#include<SDL.h>

class Obstacle
{
private:
    SDL_Rect* obstacleRect;

public:

    Obstacle()
    {
        obstacleRect = new SDL_Rect;;
        obstacleRect->h = 50;
    obstacleRect->y = 600 / 2 + 80 + 80 - obstacleRect->h ;
    obstacleRect->w = 40;
    obstacleRect->x  = 800;
    }
    ~Obstacle(){
        delete obstacleRect;
        };
    void update(int speed);

    void draw(SDL_Renderer * ren, SDL_Texture * tex);

    SDL_Rect get_rect();

};
