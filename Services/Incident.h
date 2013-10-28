#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

class Incident {

public:
  
   Incident( const std::string& type ) : m_type(type) {}

   /*virtual*/ ~Incident() {}

   const std::string& svcType()  const { return m_type; }

private:

 std::string m_type;

};

namespace IncidentType 
{

  const std::string BeginProcessing = "BeginRun";
  const std::string EndProcessing = "EndRun";

  const std::string BeginEvent = "BeginEvent";
  const std::string EndEvent   = "EndEvent";
 
}

#endif