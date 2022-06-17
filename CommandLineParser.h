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

        static inline int readCmdInt(char** begin, char** end, const std::string& option, int lower = 0, int upper = 0) {
            if (!cmdOptionExists(begin, end, option))
                return 0;
            
            std::string tmp = readCmdOption(begin, end, option);
            return strToInt(tmp, lower, upper);
        }

        // read in list of values: -option 1,2,3,4
        static inline std::vector<std::string> readCmdList(char** begin, char** end, const std::string& option, const std::string& delimiter = ",") {
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

        static inline std::vector<int> readCmdListInt(char** begin, char** end, const std::string& option, int lower = 0, int upper = 0, const std::string& delimiter = ",") {
            if (!cmdOptionExists(begin, end, option))
                return std::vector<int>();

            std::vector<std::string> s = readCmdList(begin, end, option, delimiter);
            std::vector<int> tmp;
            for (const std::string& str : s) {
                tmp.push_back(strToInt(str, lower, upper));
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

    private:
        static inline int strToInt(const std::string& str, int lower = 0, int upper = 0) {
            int tmp = 0;
            try { 
                tmp = std::stoi(str);
                if (lower != 0 || upper != 0) {
                    if (tmp < lower || tmp > upper) {
                        std::cout << "Warning: Number not in range: " << tmp << ", (" << lower << ", " << upper << ")" << std::endl;
                        return Utilities::constrain<int>(tmp, lower, upper);
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "Warning: " << e.what() << ": No input number: " << tmp << std::endl;
            }
            return tmp;
        }
    };
}