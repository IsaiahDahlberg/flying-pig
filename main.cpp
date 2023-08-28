#include <cstdlib>
#include <iostream>
#include "LTexture.cpp"
#include "pie.cpp"
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
	//Loading success flag
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

	return success;
}

void close()
{
	PIE_TEXTURE.free();
	BACKGROUND_TEXTURE.free();

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

    int countedFrames = 0;
    fpsTimer.start();

		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			quit = true;
		}

    Pie pie(120, 120);
			
		while(!quit)
		{
      capTimer.start();
			while (SDL_PollEvent(&e) != 0)
			{
				if ( e.type == SDL_QUIT)
				{
					quit = true;
				}
        pie.handleEvent(e);
		}

      float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f );
      if (avgFPS > 2000000)
      {
        avgFPS = 0;
      }

      pie.move();

			SDL_RenderClear(RENDERER);

      BACKGROUND_TEXTURE.render(0,0, RENDERER);
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
