#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

class LTimer
{
    public:
        LTimer();

        void start();
        void stop();
        void pause();
        void unpause();

        Uint32 getTicks();

        bool isStarted();
        bool isPaused();

    private:
        Uint32 mStartTicks;

        Uint32 mPausedTicks;

        bool mPaused;
        bool mStarted;
};

LTimer::LTimer()
{
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    mStarted = true;

    mPaused = false;

    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    mStarted = false;

    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    if( mStarted && !mPaused )
    {
        mPaused = true;

        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    if( mStarted && mPaused )
    {
        mPaused = false;

        mStartTicks = SDL_GetTicks() - mPausedTicks;

        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    Uint32 time = 0;

    if( mStarted )
    {
        if( mPaused )
        {
            time = mPausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}


