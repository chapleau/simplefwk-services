%module(directors="1") IncidentService
%{
#include "Services/IncidentService.h"
#include "Services/Incident.h"
#include "Services/IIncidentListener.h"
%}

%include "std_string.i"

%feature("director") PyIIncidentListener;

%include "Services/IIncidentListener.h"

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

     void addListener(IIncidentListener*, const std::string&, int priority = 0, bool single = false);

     void fireIncident (const Incident& );
     
     static IncidentService *getInstance ();
     static void kill ();
     
private:

    IncidentService();
    ~IncidentService();     

};
