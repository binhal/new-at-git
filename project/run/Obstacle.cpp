#include"Obstacle.h"

void Obstacle::update(int speed)
{
    obstacleRect->x -= speed;

}

void Obstacle::draw(SDL_Renderer * ren, SDL_Texture * tex)
{
    SDL_RenderCopy( ren, tex, NULL, obstacleRect );

}
 SDL_Rect Obstacle::get_rect()
 {
     return *obstacleRect ;
 }
