/*
 *  Timer.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 *
 */
#pragma once

namespace sdl::auxiliary {
    class Timer {
    private:
        int startTicks;
        int pausedTicks;
        
        bool paused;
        bool started;

    public:
        Timer() {
            startTicks = 0;
            pausedTicks = 0;
            paused = false;
            started = false;
        }

        void start() {
            started = true;
            paused = false;
            startTicks = SDL_GetTicks();
        }

        void pause() {
            if( started && !paused ) {
                paused = true;
                pausedTicks = SDL_GetTicks() - startTicks;
            } 
        }

        void stop() {
            started = false;
            paused = false;
        }

        void unpause() {
            if(started && paused) {
                paused = false;
                startTicks = SDL_GetTicks() - pausedTicks;
                pausedTicks = 0;
            }
        }
        
        int getTicks() {
            if(started) {
                if(paused) {
                    return pausedTicks;
                } else {
                    return SDL_GetTicks() - startTicks;
                }
            }
            return 0;
        }

        bool is_started() {
            return started;
        }

        bool is_paused() {
            return paused;
        }
    };
}