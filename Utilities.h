/*
 *  Utilities.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2012. All rights reserved.
 */
#pragma once
#include <utility>
#include <SDL.h>

namespace sdl::auxiliary {
    class Utilities
    {
    public:
        // get the angle in respect to the x-axis from a vector with x & y coordinates
        inline static float getAngle(float x, float y) {
            float theta = 0.0f;
            if ( x>0 ) {
                theta = atan(y/x);
            } else if ( x<0.0f && y>=0.0f ) {
                theta = atan(y/x) + M_PI;
            } else if ( x<0.0f && y<0.0f ) {
                theta = atan(y/x) - M_PI;
            } else if ( x==0.0f && y>0.0f ) {
                theta = M_PI/2.0f;
            } else if ( x==0.0f && y<0.0f ) {
                theta = -M_PI/2.0f;
            } else {
                theta = 0.0f;
            }
            return theta;
        }

        // easier print function
        template <typename T>
        inline static void print(T t) {
            std::cout << t << std::endl;
        }

        // recursive variadic print function
        template<typename T, typename ...Args>
        inline static void print(T t, Args ...args) {
            std::cout << t;
            print(args...);
        }

        inline static void seed(int n) {
            std::srand(n);
        }

        inline static float frand(float min, float max) {
            return min + (max - min) * ((std::rand() % 1000) / 1e3);
        }
        
        // Draw a curled line between two points on the screen renderer with a given amplitude and a number of half waves.
        inline static int curlRGBA(SDL_Renderer* renderer, const Sint16 x1, const Sint16 y1, const Sint16 x2, const Sint16 y2,
                const Sint16 amp,const Sint16 num_half_waves, int steps, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
            // the input values are short ints, but actually the calculation
            // is made with floats
            const float x1f = (float)x1;
            const float x2f = (float)x2;
            const float y1f = (float)y1;
            const float y2f = (float)y2;
            float ampf = (float)amp;
            const float theta = getAngle(x2f-x1f, y2f-y1f);;
            
            // constants
            const int npoints = 12;
            const float SQRT2 = 1.41421356f;
            const float Y1 = (2.0f*SQRT2)/7.0f - 1.0f/7.0f;
            const float Y2 = (4.0f*SQRT2)/7.0f - 2.0f/7.0f;
            const float Y3 = SQRT2/2.0f;
            const float Y4 = (3.0f*SQRT2)/7.0f + 2.0f/7.0f;
            const float PI = 3.1415926535897f;
            const float XD = 0.261799388f;
            const float width = sqrt(pow((x1f-x2f),2) + pow((y1f-y2f),2));
            const float xmu1 = width / (num_half_waves * PI);
            const float xd1 = XD * xmu1;
            
            float xf = 0.0f;
            float yf = 0.0f;
            short s[npoints*num_half_waves];
            short t[npoints*num_half_waves];
            for (int i=0; i<num_half_waves; i++) {
                int j=0;
                s[i*npoints+j] = (short)(xf+1.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y1); j++;
                s[i*npoints+j] = (short)(xf+2.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y2); j++;
                s[i*npoints+j] = (short)(xf+3.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y3); j++;
                s[i*npoints+j] = (short)(xf+4.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y4); j++;
                s[i*npoints+j] = (short)(xf+5.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
                s[i*npoints+j] = (short)(xf+6.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
                s[i*npoints+j] = (short)(xf+7.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
                s[i*npoints+j] = (short)(xf+8.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf);    j++;
                s[i*npoints+j] = (short)(xf+9.0f*xd1);  t[i*npoints+j] = (short)(yf+ampf*Y4); j++;
                s[i*npoints+j] = (short)(xf+10.0f*xd1); t[i*npoints+j] = (short)(yf+ampf*Y3); j++;
                s[i*npoints+j] = (short)(xf+11.0f*xd1); t[i*npoints+j] = (short)(yf+ampf*Y2); j++;
                s[i*npoints+j] = (short)(xf+12.0f*xd1); t[i*npoints+j] = (short)(yf+ampf*Y1); j++;
                    
                xf += (width / num_half_waves);
                ampf = - ampf;
            }
            
            short ss[npoints*num_half_waves];
            short ts[npoints*num_half_waves];
            float cosQ = cos(theta);
            float sinQ = sin(theta);
            for (int i=0; i<num_half_waves; i++) {
                for (int j=0; j<npoints; j++) {
                    ss[i*npoints+j] = cosQ*s[i*npoints+j] - sinQ*t[i*npoints+j] + x1;
                    ts[i*npoints+j] = sinQ*s[i*npoints+j] + cosQ*t[i*npoints+j] + y1;
                }
            }
            
            return bezierRGBA(renderer, ss, ts, npoints*num_half_waves, steps, r, g, b, a);
        }

        // Draw a pixel with variable width
        inline static void drawPixel(SDL_Renderer *renderer, const int x, const int y, const int width, const int scale, const int red = 0) {
            const int regPixel = 10;
            boxRGBA(renderer, x - width / 2, y - width / 2, x + width / 2 + regPixel, y + width / 2 + regPixel, scale, scale * (1 - red), scale * (1 - red), 255);
        }
        
        // Log an SDL error with some error message to the output stream of our choice
        inline static void logSDLError(std::ostream &os, const std::string &msg) {
            os << msg << " error: " << SDL_GetError() << std::endl;
        }

        // Log an SDL debug output with some message to the output stream of our choice
        inline static void logSDLDebug(std::ostream &os, const std::string &msg) {
            os << " [DEBUG]: " << msg << std::endl;
        }
        inline static void logSDLDebug(std::ostream &os, const int msg) {
            os << " [DEBUG]: " << msg << std::endl;
        }
        
        inline static void cleanup(SDL_Window *win) {
            if (!win) return;
            SDL_DestroyWindow(win);
        }

        inline static void cleanup(SDL_Renderer *ren) {
            if (!ren) return;
            SDL_DestroyRenderer(ren);
        }

        inline static void cleanup(SDL_Texture *tex) {
            if (!tex) return;
            SDL_DestroyTexture(tex);
        }

        inline static void cleanup(SDL_Surface *surf) {
            if (!surf) return;
            SDL_FreeSurface(surf);
        }
        
        
        // Recurse through the list of arguments to clean up, cleaning up
        // the first one in the list each iteration.
        template<typename T, typename... Args>
        inline static void cleanup(T *t, Args&&... args)
        {
            // Cleanup the first item in the list
            cleanup(t);
            // Clean up the remaining arguments
            cleanup(std::forward<Args>(args)...);
        }

        // Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
        // width and height
        inline static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
            //Setup the destination rectangle to be at the position we want
            SDL_Rect dst;
            dst.x = x;
            dst.y = y;
            dst.w = w;
            dst.h = h;
            SDL_RenderCopy(ren, tex, NULL, &dst);
        }

        // Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
        // the texture's width and height
        inline static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
            int w, h;
            SDL_QueryTexture(tex, NULL, NULL, &w, &h);
            renderTexture(tex, ren, x, y, w, h);
        }
    };
}