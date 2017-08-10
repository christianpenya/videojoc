#include "StringUtils.h"

namespace base
{
namespace utils
{
std::string GetFileNameExtension(const std::string& FileName)
{
    if (FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".") + 1);
    return "";
}

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

std::vector<std::string>& Split(const std::string& s, char delim, std::vector<std::string>& elems)
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

std::wstring String2WString(const std::string &str)
{
    if (str.empty()) return std::wstring();
    int sz = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), 0, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}
}
}
