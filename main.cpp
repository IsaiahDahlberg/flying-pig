#include <cstdlib>
#include <iostream>
#include "LTexture.cpp"
#include "pie.cpp"
#include "wall.h"
#include "section.h"
#include "LTimer.cpp"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

int points = 0;

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
    int hitAtTicks = 0;
    bool pause = true;

    int countedFrames = 0;
    fpsTimer.start();

		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			quit = true;
		}

    Pie pie(120, 120);
    int len = 2;
    Section sections[] = { 
      Section(320, 50),
      Section(528, 90),
    };
	
    SDL_Color textColor {0, 0, 0, 255};
    std::stringstream timeText;		
		while(!quit)
		{
      capTimer.start();
			while (SDL_PollEvent(&e) != 0)
			{
				if ( e.type == SDL_QUIT)
				{
					quit = true;
				}
	      if( e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
          if (hit == true)
          {
            if (hitAtTicks + 1000 < fpsTimer.getTicks())
            {
              hit = false;
              points = 0;
              pie.reset(120, 120);
              sections[0] = Section(320, 50);
              sections[1] = Section(528, 90);
            }
          }
        }
        pie.handleEvent(e);
		  }

      float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f );
      if (avgFPS > 2000000)
      {
        avgFPS = 0;
      }

      for(int i = 0; i < len; i += 1)
      {
        if (hit == false)
        {
          sections[i].advance(1); 
          if (sections[i].checkCollision(pie.X(), pie.Y(), 12, 32))
          {
            hit = true;
            hitAtTicks = fpsTimer.getTicks();
          }

          if (sections[i].isPastPoint(-48))
          {
            sections[i].resetTo(320);
          }

          if (sections[i].isPointEligible() && sections[i].isPastPoint(pie.X() - 32))
          {
             sections[i].pointTaken();
             points += 1;
          }
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
        WALL_TEXTURE.render(sections[i].top().X(), sections[i].top().Y(), RENDERER);
        WALL_TEXTURE.render(sections[i].bot().X(), sections[i].bot().Y(), RENDERER);
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
