#pragma once
#ifndef __H_LOGGER__
#define __H_LOGGER__

#include "Utils/Singleton.h"
#include "Utils/Types.h"

#include <vector>
#include <string>

#ifdef NO_LOGS
#define LOG_ERROR_APPLICATION( x, ... ){}
#define LOG_WARNING_APPLICATION( x, ...  ){}
#define LOG_INFO_APPLICATION( x, ...  ){}
#else 
#define LOG_ERROR_APPLICATION( x, ... )     base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogError,   x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION( x, ...  )  base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogWarning, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION( x, ...  )     base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogInfo,    x, __VA_ARGS__ )
#define LOG_ERROR_APPLICATION_IF( condition, x, ... ) if(!condition) base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogError,   x, __VA_ARGS__ )
#endif

namespace base
{
    namespace utils
    {
        class CLogger : public CSingleton<CLogger>
        {
        public:
            enum ELogLevel
            {
                eLogInfo = 0,
                eLogWarning,
                eLogError
            };
        public:
            CLogger();
            virtual ~CLogger();
            void AddNewLog(ELogLevel ll, const char* format, ...);
        };
    }
}

#endif // __H_LOGGER__