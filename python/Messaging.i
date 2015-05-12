%module Messaging
%{
#include "Services/Messaging.h"
#include "Services/MessageService.h"
%}

%include "std_string.i"

enum TLogLevel  {logERROR, logWARNING, logINFO, logDEBUG, logVERBOSE};

%include "exception.i"


%exception {
    try {
        $action
    }
    catch (const std::exception & e)
    {
        SWIG_exception(SWIG_RuntimeError, (std::string("C++ std::exception in $decl: ") + e.what()).c_str());
    }
    catch (...)
    {
        SWIG_exception(SWIG_UnknownError, "C++ anonymous exception");
    }
}


class PyMessaging {

public:

   PyMessaging(const std::string& );
   PyMessaging(const std::string&, TLogLevel);
   
   void PyLOG(const std::string&, TLogLevel, bool re_throw = false);
};


class MessageService {

  public:

    static MessageService *getInstance ();
    void SetReportingLevel(TLogLevel);

  private:
     MessageService();
     ~MessageService();     


};
