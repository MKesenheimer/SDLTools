/*
 *  CommandLineParser.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2019. All rights reserved.
 */
#pragma once
#include <string>
#include <algorithm>
#include "Utilities.h"

namespace sdl::auxiliary {
    class CommandLineParser
    {
    public:
        // reads the string after the identifier option, i.e. for arg** = -i path "path" is returned if option = "-i"
        static inline char* getCmdOption(char** begin, char ** end, const std::string& option) {
            char** itr = std::find(begin, end, option);
            if (itr != end && ++itr != end) {
                return *itr;
            }
            return 0;
        }

        // true or false, if the command line option exists
        static inline bool cmdOptionExists(char** begin, char** end, const std::string& option) {
            return std::find(begin, end, option) != end;
        }

        static inline std::string readCmdOption(char** begin, char ** end, const std::string& option) {
            std::string tmp = std::string();
            if (cmdOptionExists(begin, end, option)) {
                tmp = getCmdOption(begin, end, option);
            }
            return tmp;
        }

        static inline std::string readCmdNormalized(char** begin, char ** end, const std::string& option) {
            std::string tmp = readCmdOption(begin, end, option);
            normalizePath(tmp);
            return tmp;
        }

        static inline int readCmdInt(char** begin, char ** end, const std::string& option, int lower = 0, int upper = 0) {
            if (!cmdOptionExists(begin, end, option))
                return 0;
            
            std::string tmp = readCmdOption(begin, end, option);
            int tmp2 = 0;
            try { 
                tmp2 = std::stoi(tmp);
                if (lower != 0 || upper != 0) {
                    if (tmp2 < lower || tmp2 > upper) {
                        std::cout << "Error: Number not in range: " << tmp2 << ", (" << lower << ", " << upper << ")" << std::endl;
                        return Utilities::constrain<int>(tmp2, lower, upper);
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "Warning: " << e.what() << ": No input number: " << tmp << std::endl;
            }

            return tmp2;
        }

        // replace \ with / and // with /
        static inline void normalizePath(std::string& path) {
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