#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

class Section 
{
  public:
    Section(int x, int y);
    void advance(int vel);
    Wall top();
    Wall bot();
    int Y();
    int X();
    bool isPastPoint(int x);
    bool checkCollision(int x, int y, int h, int w);
    void resetTo(int x);
    bool isPointEligible();
    void pointTaken();
  private:
    int secX, secY;
    Wall topWall, botWall;
    bool eligiblePoint;
};

Section::Section(int x, int y)
{
  eligiblePoint = true;
  secX = x;
  secY = y;
  topWall = Wall(x, y + 95);
  botWall = Wall(x, y - 95);
}

void Section::advance(int vel)
{
  secX -= vel;
  topWall.move(vel);
  botWall.move(vel);
}

Wall Section::top()
{
  return topWall;
}

Wall Section::bot()
{
  return botWall;
}

int Section::X()
{
  return secX;
}

int Section::Y()
{
  return secY;
}

bool Section::isPastPoint(int x)
{
  return x > secX;
}


bool Section::checkCollision(int x, int y, int h, int w)
{
  return topWall.checkCollision(x, y, h, w) || botWall.checkCollision(x, y, h, w);
}

void Section::resetTo(int x)
{
  secX = x;
  eligiblePoint = true;
  int wallY = rand() % 100 + 1;
  topWall.move(x, wallY-95);
  botWall.move(x, wallY + 95);
}

bool Section::isPointEligible()
{
  return eligiblePoint;
}

void Section::pointTaken()
{
  eligiblePoint = false;
}
