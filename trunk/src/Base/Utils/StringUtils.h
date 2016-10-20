#pragma once
#ifndef _H_VISIBLE__
#define _H_VISIBLE__

#include "Defines.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <sstream>
#include <stdio.h>

namespace base {
    namespace utils
    {
         void FormatString(std::string& output, const char* format, ...)
        {
            va_list args;
            char* buffer;
            va_start(args, format);
            int len = _vscprintf(format, args) + 1;
            buffer = (char*)malloc(len * sizeof(char));
            vsprintf_s(buffer, len, format, args);
            output = buffer;
            free(buffer);
            va_end(args);
        }

         std::vector<std::string>& Split(const std::string& s, char delim,
            std::vector<std::string>& elems)
        {
            std::stringstream ss(s);
            std::string item;

            while (std::getline(ss, item, delim))
                elems.push_back(item);

            return elems;
        }

         std::vector<std::string> Split(const std::string& s, char delim)
        {
            std::vector<std::string> elems;
            Split(s, delim, elems);
            return elems;
        }
    }
}

#endif // _H_VISIBLE__