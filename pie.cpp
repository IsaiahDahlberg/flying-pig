#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class Pie 
{
    public:
		static const int WIDTH = 32;
		static const int HEIGHT = 32;
		static const int SCREEN_WIDTH = 320;
		static const int SCREEN_HEIGHT = 240;
    static const int GRAVITY_VELOCITY = 1;
    static const int GRAVITY_MAX_SPEED = 6;
		static const int VEL = 5;

		Pie(int x, int y);
		void handleEvent( SDL_Event& e );
		void move();
    int Y();
    int X();

    private:
		int pieX, pieY;
		int velY;
};

Pie::Pie(int x, int y)
{
    pieY = y;

    pieX = x;
}

void Pie::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: velY -= VEL; break;
        }
    }
}

void Pie::move()
{
  if (velY < GRAVITY_MAX_SPEED)
  {
    velY += GRAVITY_VELOCITY;
  }

  pieY += velY;

  if( ( pieY < 0 ) || (pieY + HEIGHT > SCREEN_HEIGHT ) )
  {
      pieY -= velY;
  }
}

int Pie::Y()
{
  return pieY;
}

int Pie::X()
{
  return pieX;
}
