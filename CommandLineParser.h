/*
 *  CommandLineParser.h
 *  Created by Matthias Kesenheimer on 11.01.12.
 *  Copyright 2019. All rights reserved.
 */
#pragma once
#include <string>
#include <algorithm>
#include <limits>
#include "GameLibrary/Algorithms.h"

namespace sdl::auxiliary {
    // cin with checks
    template<class T> 
    inline T ccin(std::string s) {
        T input;
        std::cout << s;
        std::cin >> input;
        while (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << s;
            std::cin >> input;
        }
        return input;
    }

    class CommandLineParser
    {
    public:
        // reads the string after the identifier option, i.e. for arg** = -i path "path" is returned if option = "-i"
        static inline char* getCmdOption(char** begin, char** end, const std::string& option) {
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

        static inline std::string readCmdOption(char** begin, char** end, const std::string& option) {
            std::string tmp = std::string();
            if (cmdOptionExists(begin, end, option)) {
                tmp = getCmdOption(begin, end, option);
            }
            return tmp;
        }

        static inline std::string readCmdNormalized(char** begin, char** end, const std::string& option) {
            std::string tmp = readCmdOption(begin, end, option);
            normalizePath(tmp);
            return tmp;
        }

        template<typename T>
        static inline T readCmdOption(char** begin, char** end, const std::string& option, T lower = 0, T upper = 0) {
            if (!cmdOptionExists(begin, end, option))
                return 0;
            
            std::string tmp = readCmdOption(begin, end, option);
            return Algorithms::strTo<T>(tmp, lower, upper);
        }

        // read in list of values: -option 1,2,3,4
        static inline std::vector<std::string> readCmdOptionList(char** begin, char** end, const std::string& option, const std::string& delimiter = ",") {
            if (!cmdOptionExists(begin, end, option))
                return std::vector<std::string>();

            std::string s = readCmdOption(begin, end, option);
            std::vector<std::string> tmp2;
            size_t pos = 0;
            std::string token;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                tmp2.push_back(token);
                s.erase(0, pos + delimiter.length());
            }
            tmp2.push_back(s);
            return tmp2;
        }

        template<typename T>
        static inline std::vector<T> readCmdOptionList(char** begin, char** end, const std::string& option, T lower = 0, T upper = 0, const std::string& delimiter = ",") {
            if (!cmdOptionExists(begin, end, option))
                return std::vector<T>();

            std::vector<std::string> s = readCmdOptionList(begin, end, option, delimiter);
            std::vector<T> tmp;
            for (const std::string& str : s) {
                tmp.push_back(Algorithms::strTo<T>(str, lower, upper));
            }
            return tmp;
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