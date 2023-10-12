#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class Wall
{
  public:
    static const int WIDTH = 48;
    static const int HEIGHT = 95;
    Wall(int x, int y);
    Wall();
    void move(int vel);
    void move(int x, int y);
    int Y();
    int X();
    bool checkCollision(int x, int y, int h, int w);
  private:
    int wallX, wallY;
};

Wall::Wall()
{
  wallX = 0;
  wallY = 0;
}

Wall::Wall(int x, int y)
{
  wallX = x;
  wallY = y;
}

void Wall::move(int vel)
{
  wallX -= vel;
}

void Wall::move(int x, int y)
{
  wallX = x;
  wallY = y;
}

int Wall::X()
{
  return wallX;
}

int Wall::Y()
{
  return wallY;
}

bool Wall::checkCollision(int x, int y, int h, int w)
{
    int leftPie, leftWall,
        rightPie, rightWall,
        topPie, topWall,
        bottomPie, bottomWall;

    leftPie = wallX;
    rightPie = wallX + WIDTH;
    topPie = wallY;
    bottomPie = wallY + HEIGHT;

    leftWall = x;
    rightWall = x+w;
    topWall = y;
    bottomWall = y+h;

    return !(bottomPie <= topWall ||
      topPie >= bottomWall ||
      rightPie <= leftWall ||
      leftPie >= rightWall );
}
