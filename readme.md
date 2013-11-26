#Simple framework - Services

``simplefwk-services`` is a small package containing a simple event handling service as well as a lightweight message service that is intended to be used as a message logger. Native C++ interfaces are provided. Python interfaces are also available and automatically generate by [swig][].

##Installation

A makefile is provided for unix-like operating systems (tested on Mac OS X 10.7.5). Requirements: 
* C++11 compliant c++ compiler (e.g. g++ 4.8)
* Python 2.x (e.g. version 2.7.1)
* swig 2.x 

All the above can be obtained through [macports][] for Max OS X systems.

## Usage

### Message Service
 
The Message service is implemented as a singleton that outputs formatted text messages to a user defined stream. It keeps track of the number of messages (and levels) that are issued. Classes that require the use of this logging mechanism simply need to inherit from the ``Messaging`` class. :

```c++
#include "Services/Messaging.h"
#include <iostream>

class tool : public Messaging {

 public:
 // n is name of instance
 tool(const std::string & n) : Messaging(n) {}

 void foo() { LOG("Here's an INFO  message from "<<this->name(), logINFO); }
 void bar() { LOG("Here's a WARNING message from "<<this->name(), logWARNING); }
 void foobar() { LOG("Here's an ERROR message from "<<this->name(), logERROR); }

};

int main() {

tool mytoolA("MyToolA");
tool mytoolB("MyToolB");

mytoolA.foo();
mytoolB.bar();

try {
 mytoolA.foobar();
}
catch(const std::exception & e) {
 std::cerr<<"caught exception: "<<e.what()<<std::endl;
}

return 0;
}
```

will produce the following output:

![Alt console output][msgoutput]

It is also possible to specifify a minimum log level through ``Messaging``'s alternate constructor. 

An error message throws a run time exception by default that is necesary to catch. 

### Incident Service

The Incident Service is a simple implementation of the [observer design pattern] [obs]. It is based on an implementation available in a software framework (known as [Gaudi][]) used for high energy physics experiments. 

The service is implemented as a singleton. Observers (or _listeners_) can register themselves to the incident service for any type of events (or _incidents_). When fireing a particular incident, all registered listeners are notified. Listeners are concrete implementation of the ``IIncidentListener`` interface, e.g.: 

```c++
#include "Services/Messaging.h"
#include "Services/IIncidentListener.h"
#include "Services/IncidentService.h"
#include <iostream>

class tool : public Messaging, virtual public IIncidentListener {

 public:
 // n is name of instance
 tool(const std::string & n) : Messaging(n) {}

 //concrete implementation
 void handle(const Incident& inc) { foo(inc.svcType()); }

 protected: 
 void foo(const std::string & str) { LOG("Here's an INFO message from incident "<<str, logINFO); }

};

int main() {

tool mytoolA("MyToolA");

//get an handle on the singleton
IncidentService * inc_svc = IncidentService::getInstance();
//set log level to DEBUG
inc_svc->SetReportingLevel(logDEBUG);
//add listeners
inc_svc->addListener(&mytoolA, "TestEvent");
inc_svc->addListener(&mytoolA, "AnotherTestEvent");
//fire!
inc_svc->fireIncident(Incident("TestEvent"));
inc_svc->fireIncident(Incident("AnotherTestEvent"));

//clean exit
inc_svc->kill();
return 0;
}
```


[swig]: http://swig.org
[macports]: http://www.macports.org/
[obs]: http://en.wikipedia.org/wiki/Observer_pattern
[Gaudi]: http://cern.ch/gaudi
[msgoutput]: https://raw.github.com/chapleau/simplefwk-services/master/doc/console.png
