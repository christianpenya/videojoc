#ifndef _BIN_FILE_READER_HH_
#define _BIN_FILE_READER_HH_

#include <string>
#include <fstream>
#include <Windows.h>

#include <cstdio>

#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Utils\CheckedDelete.h"

namespace base
{
namespace utils
{
class CBinFileReader
{
public:
    CBinFileReader(const std::string& aFileName)
        : mFilename(aFileName)
        , mStream(nullptr)
    {
    }

    virtual ~CBinFileReader()
    {
    }

    bool Open()
    {
        bool lOk;
        if (!mFilename.empty())
        {
            fopen_s(&mStream, mFilename.c_str(), "rb");
            lOk = mStream != nullptr;
        }
        return lOk;
    }

    void Close()
    {
        std::fclose(mStream);
    }

    template < typename T > T Read()
    {
        T lBuffer;
        std::fread(&lBuffer, sizeof(T), 1, mStream);
        return lBuffer;
    }

    void* ReadRaw(size_t aNumBytes)
    {
        void *lAddr = (void *)malloc(aNumBytes);
        ZeroMemory(lAddr, aNumBytes);
        std::fread(lAddr, aNumBytes, 1, mStream);
        return lAddr;
    }

    template<> std::string CBinFileReader::Read<std::string>()
    {
        unsigned short lCount = 0;
        std::fread(&lCount, sizeof(unsigned short), 1, mStream);
        ++lCount; // Handle \0
        char* lString = (char *)malloc(sizeof(char) * (lCount));
        ZeroMemory(lString, sizeof(char) * lCount);
        std::fread(lString, sizeof(char) * lCount, 1, mStream);
        std::string lStandardString(lString);
        free(lString);
        return lStandardString;
    }

protected:
    DISALLOW_COPY_AND_ASSIGN(CBinFileReader);
    std::string mFilename;
    std::FILE* mStream;
};
}
}

#endif
