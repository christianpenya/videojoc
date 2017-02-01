#include "Logger.h"
#include "Utils/StringUtils.h"
#include "Utils/Defines.h"

#include "Utils/EnumToString.h"

#include <iostream>
#include <windows.h> // WinApi header

Begin_Enum_String(base::utils::CLogger::ELogLevel)
{
    Enum_String_Id(base::utils::CLogger::eLogInfo, "Info");
    Enum_String_Id(base::utils::CLogger::eLogWarning, "Warning");
    Enum_String_Id(base::utils::CLogger::eLogError, "Error");
}
End_Enum_String;

namespace base{
    namespace utils{

        

        CLogger::CLogger()
        {
        }

        CLogger::~CLogger()
        {
        }

        void CLogger::AddNewLog(ELogLevel aLevel, const char* format, ...)
        {
            va_list args;
            char* buffer;
            va_start(args, format);
            int len = _vscprintf(format, args) + 1;
            buffer = (char*)malloc(len * sizeof(char));
            vsprintf_s(buffer, len, format, args);
            std::string lTag = EnumString<ELogLevel>::ToStr(aLevel);
            std::string lMsg;
            FormatString(lMsg, "[%s]%s\n", lTag.c_str(), buffer);
            OutputDebugString(lMsg.c_str());
            free(buffer);
            va_end(args);
        }
    }
}