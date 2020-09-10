#pragma once
#include <string>

namespace sdl::auxiliary {
    class CommandLineParser
    {
    public:
        /// liest den string nach dem identifier option, d.h. fuer arg** = -i path wird "path"
        /// zurueckgegeben, wenn option = "-i"
        static char* getCmdOption(char** begin, char ** end, const std::string& option) {
            char ** itr = std::find(begin, end, option);
            if (itr != end && ++itr != end)
            {
                return *itr;
            }
            return 0;
        }

        /// true or false, falls die Kommandozeilenoption exisitert
        static bool cmdOptionExists(char** begin, char** end, const std::string& option) {
            return std::find(begin, end, option) != end;
        }

        /// ersetzt \ durch / und // durch /
        static void normalizePath(std::string& path) {
            const size_t maxiter = path.size();
            // ersetze \\ durch /
            size_t pos = path.find("\\");
            for (size_t i = 0; i < maxiter && pos != std::string::npos; ++i)
            {
                path.replace(pos, 1, "/");
                pos = path.find("\\");
            }

            // ersetze // durch /
            pos = path.find("//");
            for (size_t i = 0; i < maxiter && pos != std::string::npos; ++i)
            {
                std::string start = path.substr(0, pos + 1);
                std::string rest = path.substr(pos + 2);
                //std::cout << start+ rest << std::endl;
                path = start + rest;
                pos = path.find("//");
            }
        }
    };
}