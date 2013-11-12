#include "Services/Messaging.h"
#include "Services/MessageService.h"
#include <exception>
#include <stdexcept>

bool Messaging::m_hara_kiri = false;

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

   if (!m_hara_kiri) {
     //important to set this to true now because MessageService inherits from Messaging so this
     //destructor is called during MessageService::kill()
     m_hara_kiri = true;

     MessageService *  svc = MessageService::getInstance();
     if (svc) svc->kill();
     
   }
  
}