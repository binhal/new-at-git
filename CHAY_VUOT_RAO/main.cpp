#include<iostream>
#include<string>
 #include <conio.h>
using namespace std;



class chay_vuot_rao
{
    private:
        float position = 0;
        float velocity = 0;
        float acceleration = 0;
        float gravity = 100;
        float elapse_time = 0.1f;
    protected:
        bool userCreate()
        {
            return true;
        }
        bool update()
        {
            //GAME
            if(getch() == 32)
            {
                acceleration = 0;
                velocity = -gravity/4.0f;
            }
            else
            {
                acceleration += gravity*elapse_time;
                velocity += acceleration*elapse_time;
                position += velocity*elapse_time;
            }
            // DRAW man

        }
};


int main()
{
    return 0;
}
