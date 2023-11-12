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
		static const int HEIGHT = 12;
		static const int SCREEN_WIDTH = 320;
		static const int SCREEN_HEIGHT = 240;
    static const int GRAVITY_VELOCITY = 1;
    static const int GRAVITY_MAX_SPEED = 3;
		static const int FLAP_THRUST = -8;

		Pie(int x, int y);
		void handleEvent( SDL_Event& e );
		void move();
    int Y();
    int X();
    bool checkCollision(int wallX, int wallY, int wallH, int wallW);
    void reset(int x, int y);

    private:
		int pieX, pieY;
		int velY;
};

Pie::Pie(int x, int y)
{
    velY = 0;
    pieY = y;

    pieX = x;
}

void Pie::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
  {
    if (velY > FLAP_THRUST)
    {
      velY += FLAP_THRUST; 
      if (velY > FLAP_THRUST)
      {
        velY = FLAP_THRUST;
      }
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

  if( ( pieY + HEIGHT < 0 ) || (pieY + HEIGHT > SCREEN_HEIGHT ) )
  {
    pieY -= velY;
    velY = 0;
  }

  printf("VelY: %d\n", velY);
}

int Pie::Y()
{
  return pieY;
}

int Pie::X()
{
  return pieX;
}

bool Pie::checkCollision(int wallX, int wallY, int wallH, int wallW)
{
    int leftPie, leftWall,
        rightPie, rightWall,
        topPie, topWall,
        bottomPie, bottomWall;

    leftPie = pieX;
    rightPie = pieX + WIDTH;
    topPie = pieY;
    bottomPie = pieY - HEIGHT;

    leftWall = wallX;
    rightWall = wallX + wallW;
    topWall = wallY;
    bottomWall = wallY + wallH;

    return !(bottomPie <= topWall ||
      topPie >= bottomWall ||
      rightPie <= leftWall ||
      leftPie >= rightWall );
}

void Pie::reset(int x, int y)
{
  pieX = x;
  pieY = y;
  velY = 0;
}
