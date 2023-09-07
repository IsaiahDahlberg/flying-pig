#include <cstdlib>
#include <iostream>
#include "LTexture.cpp"
#include "pie.cpp"
#include "wall.h"
#include "LTimer.cpp"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

bool init();
bool loadMedia();
void close();
void handleKeyDown(SDL_Event e);

SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;
SDL_Surface* SCREEN_SURFACE = NULL;
SDL_Texture* CURRENT_TEXTURE = NULL;

LTexture BACKGROUND_TEXTURE;
LTexture PIE_TEXTURE;
LTexture WALL_TEXTURE;

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize: %s\n", SDL_GetError());
		return false;
	}

	WINDOW = SDL_CreateWindow(
			"flappy pie", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN
		);

	if (WINDOW == NULL)
	{
		printf("Window failed to instantiate: %s\n", SDL_GetError());
		return false;
	}

	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	if (RENDERER == NULL)
	{
		printf("Renderer failed to instantiate: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);

	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) & imageFlags))
	{
		printf("SDL_Image could not initialize. Error: %s\n", IMG_GetError());
		return false;
	}

	SCREEN_SURFACE = SDL_GetWindowSurface(WINDOW);

	return true;
}

bool loadMedia()
{
  bool success = true;

  if (!PIE_TEXTURE.loadFromFile("assets/pie.png", RENDERER))
  {
    printf("Failed to load pie texture!\n");
    success = false;
  }

  if (!BACKGROUND_TEXTURE.loadFromFile("assets/background.png", RENDERER))
  {
    printf("Failed to load pie texture!\n");
    success = false;
  }

  if (!WALL_TEXTURE.loadFromFile("assets/wall.png", RENDERER))
  {
    printf("Failed to load wall texture!\n");
    success = false;
  }

	return success;
}

void close()
{
	PIE_TEXTURE.free();
	BACKGROUND_TEXTURE.free();
	WALL_TEXTURE.free();

	SDL_DestroyTexture(CURRENT_TEXTURE);

	SDL_DestroyRenderer(RENDERER);
	RENDERER = NULL;

	SDL_DestroyWindow(WINDOW);
	WINDOW = NULL;
		
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize.\n");
	}
	else
	{
		bool quit = false;
		SDL_Event e;
    LTimer fpsTimer;
    LTimer capTimer;
    bool hit = false;

    int countedFrames = 0;
    fpsTimer.start();

		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			quit = true;
		}

    Pie pie(120, 120);
    int len = 4;
    Wall walls[] = { 
      Wall(320, 170),
      Wall(320, -30),
      Wall(480, 190),
      Wall(480, -60),
    };
			
		while(!quit)
		{
      capTimer.start();
			while (SDL_PollEvent(&e) != 0)
			{
				if ( e.type == SDL_QUIT)
				{
					quit = true;
				}
	      if( e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_r)
        {
          hit = false;
          pie.reset(120, 120);
        }
        pie.handleEvent(e);
		}

      float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f );
      if (avgFPS > 2000000)
      {
        avgFPS = 0;
      }

      for(int i = 0; i < len; i += 2)
      {
        walls[i].move(); 
        walls[i+1].move(); 
        if (pie.checkCollision(walls[i].X(), walls[i].Y(), 95, 48) ||
            pie.checkCollision(walls[i + 1].X(), walls[i + 1].Y(), 95, 48))
        {
          hit = true;
        }

        if (walls[i].X() <= -30)
        {
          int wallY = rand() % 100 + 1;
          walls[i].move(330, wallY-95);
          walls[i + 1].move(330, wallY + 95);
        }
      }

      if (!hit)
      {
        pie.move();
      }

			SDL_RenderClear(RENDERER);
      BACKGROUND_TEXTURE.render(0,0, RENDERER);

      for(int i = 0; i < len; i++)
      {
        WALL_TEXTURE.render(walls[i].X(), walls[i].Y(), RENDERER);
      }

      PIE_TEXTURE.render(pie.X(), pie.Y(), RENDERER);
 			SDL_RenderPresent(RENDERER);
      ++countedFrames;

      int frameTicks = capTimer.getTicks();
      if (frameTicks < SCREEN_TICKS_PER_FRAME)
      {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		  }
	  }
  }

	close();

	return 0;
}
