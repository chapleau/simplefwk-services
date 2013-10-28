#include "Services/Messaging.h"
#include "Services/MessageService.h"


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
   
   thelog_ptr->Get()<< name()<<" : "<<buf_str;
   
}

