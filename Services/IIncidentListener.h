#ifndef INCIDENTLISTENER_H
#define INCIDENTLISTENER_H

#include "Services/Incident.h"


class IIncidentListener  {


public:
  virtual void handle(const Incident&) = 0;
  
  //prevent delete on bare IIncidentListener pointer
protected:

 ~IIncidentListener(){}
};

#endif
