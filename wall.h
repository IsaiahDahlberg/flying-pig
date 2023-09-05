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
    static const int VEL = 1;
    Wall(int x, int y);
    void move();
    void move(int x, int y);
    int Y();
    int X();

  private:
    int wallX, wallY;
};

Wall::Wall(int x, int y)
{
  wallX = x;
  wallY = y;
}

void Wall::move()
{
  wallX -= VEL;
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
