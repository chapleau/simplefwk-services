//source: http://www.drdobbs.com/cpp/logging-in-c/201804215
#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>
#include "Services/log_levels.h"


inline std::string NowTime();

template <typename T>
class Log
{
public:
    Log() {};
    Log(TLogLevel level) : m_lvl(level) {};
    virtual ~Log();
    std::ostringstream& Get(TLogLevel);
    std::ostringstream& Get();
public:
    //static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);
protected:
    std::ostringstream os;
    TLogLevel m_lvl;
private:
    Log(const Log&);
    Log& operator =(const Log&);
};



template <typename T>
std::ostringstream& Log<T>::Get(TLogLevel level)
{

    std::string color_prefix = "\e[1;92m";
    if (level == logERROR ) color_prefix = "\e[1;91m";
    if (level == logWARNING ) color_prefix = "\e[1;93m";

    std::string color_suffix = "\e[0m";
    
    os << "- " << NowTime();
    os << " " << color_prefix<< ToString(level)<<color_suffix << ": ";
    //os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
    return os;
}

template <typename T>
std::ostringstream& Log<T>::Get()
{

    std::string color_prefix = "\e[1;92m";
    if (m_lvl == logERROR ) color_prefix = "\e[1;91m";
    if (m_lvl == logWARNING ) color_prefix = "\e[1;93m";

    std::string color_suffix = "\e[0m";
    os << "- " << NowTime();
    os << " " << color_prefix << ToString(m_lvl) <<color_suffix << ": ";
    return os;
}

template <typename T>
Log<T>::~Log()
{
    os << std::endl;
    T::Output(os.str());
}

//template <typename T>
//TLogLevel& Log<T>::ReportingLevel()
//{
//    static TLogLevel reportingLevel = logINFO;
//    return reportingLevel;
//}

template <typename T>
std::string Log<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "VERBOSE"};
    return buffer[level];
}

template <typename T>
TLogLevel Log<T>::FromString(const std::string& level)
{
    if (level == "VERBOSE")
        return logVERBOSE;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "INFO")
        return logINFO;
    if (level == "WARNING")
        return logWARNING;
    if (level == "ERROR")
        return logERROR;
    Log<T>().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}

class Output2FILE
{
public:
    static FILE*& Stream();
    static void Output(const std::string& msg);
};

inline FILE*& Output2FILE::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

inline void Output2FILE::Output(const std::string& msg)
{   
    FILE* pStream = Stream();
    if (!pStream)
        return;
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}



class FILELog : public Log<Output2FILE> {

public:
  FILELog(TLogLevel level) : Log<Output2FILE>(level) {};

};
//typedef Log<Output2FILE> FILELog;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logVERBOSE
#endif

#define FILE_LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream()) ; \
    else FILELog().Get(level)

#include <sys/time.h>

inline std::string NowTime()
{
    char buffer[11];
    time_t t;
    time(&t);
    tm r = {0};
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, 0);
    char result[100] = {0};
    std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000); 
    return result;
}


#endif //__LOG_H__
