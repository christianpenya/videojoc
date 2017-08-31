#include <string>;
#include <sys\stat.h>
#include <Windows.h>
#include <cassert>

namespace base
{
namespace utils
{

bool DoesFileExist(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

bool IsFileOlder(std::string aFile1, std::string aFile2)
{
    HANDLE oldFile = CreateFile(aFile1.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE newFile = CreateFile(aFile2.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    FILETIME ftCreate1, ftAccess1, ftWrite1;
    auto lOk1 = GetFileTime(oldFile, &ftCreate1, &ftAccess1, &ftWrite1);

    FILETIME ftCreate2, ftAccess2, ftWrite2;
    auto lOk2 = GetFileTime(newFile, &ftCreate2, &ftAccess2, &ftWrite2);

    assert(lOk1 && lOk2);

    bool lOut = CompareFileTime(&ftWrite1, &ftWrite2) == -1L ? true : false;

    SYSTEMTIME stUTC1, stUTC2, stLocal;
    DWORD dwRet;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite1, &stUTC1);
    FileTimeToSystemTime(&ftWrite2, &stUTC2);

    CloseHandle(oldFile);
    CloseHandle(newFile);

    return lOut;
}

}
}
