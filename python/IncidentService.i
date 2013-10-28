%module IncidentService
%{
#include "Services/IncidentService.h"
#include "Services/Incident.h"
%}

%include "std_string.i"


class Incident {

public:
  
   Incident( const std::string& type );

   const std::string& svcType()  const;

};

namespace IncidentType 
{

  const std::string BeginProcessing = "BeginRun";
  const std::string EndProcessing = "EndRun";

  const std::string BeginEvent = "BeginEvent";
  const std::string EndEvent   = "EndEvent";
 
}


class IncidentService {


public:

     void fireIncident (const Incident& );
     
     static IncidentService *getInstance ();
     static void kill ();
     
private:

    IncidentService();
    ~IncidentService();     

};
