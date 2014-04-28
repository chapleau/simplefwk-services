#ifndef INCIDENTLISTENER_H
#define INCIDENTLISTENER_H

#include "Services/Incident.h"


class IIncidentListener  {


public:
  virtual void handle(const Incident&) = 0;
  
  //prevent delete on bare IIncidentListener pointer
protected:

 virtual ~IIncidentListener(){}
};

//python interface

class PyIIncidentListener: virtual public IIncidentListener {

public:

 //no-op
 virtual void handle(const Incident&) {};

 virtual ~PyIIncidentListener(){}

};


#endif
