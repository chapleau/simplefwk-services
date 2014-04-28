#ifndef MESSAGING_H
#define MESSAGING_H

#include <sstream>
#include "Services/log_levels.h"

#include "Services/INamed.h"

class Messaging : virtual public INamed
{

public:

  const std::string&  name() const { return m_name; }
  void setname(const std::string& n) { m_name = n; }
  
  void SetReportingLevel(TLogLevel level) {m_lvl = level;}

protected :

 void MSG(std::ostream &, TLogLevel);

 std::ostringstream& BUF() {m_msg_buf.str(""); return m_msg_buf;}
 
 std::ostringstream m_msg_buf;
 
 ~Messaging();
 
 
 protected:
 
   Messaging(const std::string& n);
   Messaging(const std::string& n, TLogLevel lvl );
   std::string m_name;
   
   bool dolog(TLogLevel lvl) { return lvl <= m_lvl; }
   
   
   static unsigned int m_obj_count;
   
private:

TLogLevel m_lvl;
 
};

#define LOG(str, level) \
    if (dolog(level)) Messaging::MSG(Messaging::BUF()<<str, level);


//for the python interface

class PyMessaging: virtual public Messaging
{

public:

   PyMessaging(const std::string& n) : Messaging(n) {} ;
   PyMessaging(const std::string& n, TLogLevel lvl ) : Messaging(n, lvl) {} ;
   
   void PyLOG(const std::string& str, TLogLevel level) {
       //catch exception
       try {
         if (dolog(level)) Messaging::MSG(Messaging::BUF()<<str, level);
       }
       catch(const std::exception&) {}
   }
   
};

#endif
