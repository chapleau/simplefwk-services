#Simple framework - Services

``simplefwk-services`` is a small package containing a simple event callback service as well as a lightweight message service that is intended to be used as a message logger. Native C++ interfaces are provided. Python interfaces are also available and automatically generate by [swig][].

##Installation

A makefile is provided for unix-like operating systems (tested on Mac OS X 10.7.5). Requirements: 
* C++11 compliant c++ compiler (e.g. g++ 4.8)
* Python 2.x (e.g. version 2.7.1)
* swig 2.x 

All the above can be obtained through [macports][] for Max OS X systems.

## Usage

# Message Service
 
The Message service is implemented as a singleton that outputs formatted text messages to a user defined stream. It keeps track of the number of messages (and levels) that are issued. Classes that require the use of the logging mechanism simply need to inherit from the ``Messaging`` class. :

```c++
#include "Services/Messaging.h"
#include <iostream>

class tool : public Messaging {

 public:
 // n is name of instance
 tool(const std::string & n) : Messaging(n) {}

 void foo() {
    LOG("Here's an INFO  message from "<<this->name(), logINFO);
 }

 void bar() {
    LOG("Here's a WARNING message from "<<this->name(), logWARNING);
 }

 void foobar() {
    LOG("Here's an ERROR message from "<<this->name(), logERROR);
 }


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



# Incident Service

The Incident Service is a simple implementation of the [observer design pattern] [obs]. It is based on an implementation available in a software framework (known as [Gaudi][]) used for high energy physics experiments. 



[swig]: http://swig.org
[macports]: http://www.macports.org/
[obs]: http://en.wikipedia.org/wiki/Observer_pattern
[Gaudi]: http://cern.ch/gaudi
[msgoutput]: chapleau/simplefwk-services/doc/console.png
