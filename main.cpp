#include <cstdlib>
#include <iostream>
#include "perlin.c"
#include "LTexture.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
 	KEY_PRESS_SURFACE_UP,
 	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

bool init();
bool loadMedia();
void close();
void handleKeyDown(SDL_Event e);

SDL_Surface* loadSurface( std::string path);
SDL_Texture* loadTexture( std::string path);

SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;
SDL_Surface* SCREEN_SURFACE = NULL;
SDL_Surface* HELLO_WORLD = NULL;
SDL_Surface* CURRENT_SURFACE = NULL;
SDL_Texture* CURRENT_TEXTURE = NULL;
SDL_Texture* KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_TOTAL];

LTexture gFooTexture;
LTexture gBackgroundTexture;

LTexture gSpriteSheetTexture;
SDL_Rect gSpriteClips[4];

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize: %s\n", SDL_GetError());
		return false;
	}

	WINDOW = SDL_CreateWindow(
			"alpha", 
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

	if (!gFooTexture.loadFromFile("assets/dude.png", RENDERER))
	{
		printf("failed to load foo\n");
		success = false;
	}

  if (!gSpriteSheetTexture.loadFromFile("assets/spritesheet.png", RENDERER))
  {
    printf("Failed to load sprite sheet texture!\n");
    success = false;
  }
  else
  {
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 20;
    gSpriteClips[0].h = 20;
    
    gSpriteClips[1].x = 30;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = 20;
    gSpriteClips[1].h = 20;
  }

	//Load default surface
	KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_DEFAULT ] = loadTexture( "assets/test.bmp" );
	if( KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
	{
		printf( "Failed to load default image!\n" );
		success = false;
	}

	//Load up surface
	KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_UP ] = loadTexture( "assets/up.bmp" );
	if( KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_UP ] == NULL )
	{
		printf( "Failed to load up image!\n" );
		success = false;
	}

	//Load down surface
	KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_DOWN ] = loadTexture( "assets/down.bmp" );
	if( KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_DOWN ] == NULL )
	{
		printf( "Failed to load down image!\n" );
		success = false;
	}

	//Load left surface
	KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_LEFT ] = loadTexture( "assets/left.bmp" );
	if( KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_LEFT ] == NULL )
	{
		printf( "Failed to load left image!\n" );
		success = false;
	}

	//Load right surface
	KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_RIGHT ] = loadTexture( "assets/right.png" );
	if( KEY_PRESS_SURFACES[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
	{
		printf( "Failed to load right image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	gFooTexture.free();
	SDL_DestroyTexture(CURRENT_TEXTURE);

	SDL_DestroyRenderer(RENDERER);
	RENDERER = NULL;

	SDL_FreeSurface(HELLO_WORLD);
	HELLO_WORLD = NULL;

	SDL_DestroyWindow(WINDOW);
	WINDOW = NULL;
		
	IMG_Quit();
	SDL_Quit();
}

void handleKeyDown(SDL_Event e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			CURRENT_TEXTURE  = KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_UP];
			break;
		case SDLK_DOWN:
			CURRENT_TEXTURE = KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_DOWN];
			break;
		case SDLK_LEFT:
			CURRENT_TEXTURE = KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_LEFT];
			break;
		case SDLK_RIGHT:
			CURRENT_TEXTURE= KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_RIGHT];
			break;
		default:
			CURRENT_TEXTURE = KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_DEFAULT];
			break;
	}
}

SDL_Texture* loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;

	// This code looks similar to loadSurface. Could be refactored for less code.
	SDL_Surface* LOADED_SURFACE = IMG_Load(path.c_str());
	if (LOADED_SURFACE == NULL)
	{
		printf("Unable to load image %s. Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(RENDERER, LOADED_SURFACE);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s. Error: %s\n", path.c_str(), SDL_GetError());

			SDL_FreeSurface(LOADED_SURFACE);
		}
	}

	return newTexture;
}

SDL_Surface* loadSurface(std::string path)
{
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* LOADED_SURFACE = IMG_Load(path.c_str());
	if (LOADED_SURFACE == NULL)
	{
		printf("Unable to load image %s. Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(LOADED_SURFACE, SCREEN_SURFACE-> format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s. Error: %s\n", path.c_str(), SDL_GetError());

			SDL_FreeSurface(LOADED_SURFACE);
		}
	}

	return optimizedSurface;
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

		if (!loadMedia())
		{
			printf("Failed to load media!\n");
			quit = true;
		}
			
		CURRENT_TEXTURE = KEY_PRESS_SURFACES[KEY_PRESS_SURFACE_DEFAULT];

		while(!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if ( e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					handleKeyDown(e);
				}
			}

			SDL_RenderClear(RENDERER);

			SDL_RenderCopy(RENDERER, CURRENT_TEXTURE, NULL, NULL);

			gFooTexture.render(240, 190, RENDERER);

      gSpriteSheetTexture.render(0,0,RENDERER,&gSpriteClips[0]);

      gSpriteSheetTexture.render(SCREEN_WIDTH-gSpriteClips[1].w,0,RENDERER,&gSpriteClips[1]);

			SDL_RenderPresent(RENDERER);

			
//			SDL_BlitSurface(CURRENT_SURFACE, NULL, SCREEN_SURFACE, NULL);

//			SDL_UpdateWindowSurface(WINDOW);
		}
	}

	close();

	return 0;
}
