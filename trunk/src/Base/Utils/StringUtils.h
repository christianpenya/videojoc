#pragma once
#ifndef _String_Utils_H_
#define _String_Utils_H_

#include "Defines.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <sstream>
#include <stdio.h>

namespace base {
    namespace utils
    {
		void FormatString(std::string& output, const char* format, ...);
		std::vector<std::string>& Split(const std::string& s, char delim, std::vector<std::string>& elems);
	    std::vector<std::string> Split(const std::string& s, char delim);
        std::string GetFileNameExtension(const std::string& FileName);
    }
}

#endif // _H_VISIBLE__