#include "Services/Messaging.h"
#include "Services/MessageService.h"
#include <exception>
#include <stdexcept>

unsigned int Messaging::m_obj_count = 0;

void Messaging::MSG(std::ostream & buf_stream, TLogLevel level) {

   std::string buf_str;
   try {
     buf_str = dynamic_cast<std::ostringstream&>(buf_stream).str();
   } catch ( std::bad_cast exp ) {
     buf_str = "bad_cast caught: " + std::string(exp.what());
     level = logERROR;
   }

   MessageService *  svc = MessageService::getInstance();
   
   std::shared_ptr<FILELog> thelog_ptr = svc->MSG(level, (const INamed*)this);
   
   
   thelog_ptr->Get() << name()  <<" : "<<buf_str;
   
   if (level == logERROR) {
      thelog_ptr.reset(); //so that message is flushed into output stream
      throw std::runtime_error("MessageService caught an ERROR message");
   }
   
}

Messaging::~Messaging() {

   m_obj_count--;

   //if one left, it is the Message Service itself !   
   if (m_obj_count == 1 && MessageService::isAlive()) {
      
       MessageService *  svc = MessageService::getInstance();
       if (svc) svc->kill();
   
   }

}

Messaging::Messaging(const std::string& n) : m_name(n), m_lvl(logINFO) {

   m_obj_count++;
   
}

Messaging::Messaging(const std::string& n, TLogLevel lvl ) : m_name(n), m_lvl(lvl) {

   m_obj_count++;
}






