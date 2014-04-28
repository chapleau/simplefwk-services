%module Messaging
%{
#include "Services/Messaging.h"
%}

%include "std_string.i"

enum TLogLevel  {logERROR, logWARNING, logINFO, logDEBUG, logVERBOSE};

class PyMessaging {

public:

   PyMessaging(const std::string& );
   PyMessaging(const std::string&, TLogLevel);
   
   void PyLOG(const std::string&, TLogLevel);
};
