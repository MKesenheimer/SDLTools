/*
 *  CommandLineParser.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2019. All rights reserved.
 */
#pragma once
#include <string>

namespace sdl::auxiliary {
    class CommandLineParser
    {
    public:
        // reads the string after the identifier option, i.e. for arg** = -i path "path" is returned if option = "-i"
        static char* getCmdOption(char** begin, char ** end, const std::string& option) {
            char ** itr = std::find(begin, end, option);
            if (itr != end && ++itr != end) {
                return *itr;
            }
            return 0;
        }

        // true or false, if the command line option exists
        static bool cmdOptionExists(char** begin, char** end, const std::string& option) {
            return std::find(begin, end, option) != end;
        }

        // replace \ with / and // with /
        static void normalizePath(std::string& path) {
            const size_t maxiter = path.size();
            // replace // with /
            size_t pos = path.find("\\");
            for (size_t i = 0; i < maxiter && pos != std::string::npos; ++i) {
                path.replace(pos, 1, "/");
                pos = path.find("\\");
            }

            // replace // with /
            pos = path.find("//");
            for (size_t i = 0; i < maxiter && pos != std::string::npos; ++i) {
                std::string start = path.substr(0, pos + 1);
                std::string rest = path.substr(pos + 2);
                //std::cout << start+ rest << std::endl;
                path = start + rest;
                pos = path.find("//");
            }
        }
    };
}