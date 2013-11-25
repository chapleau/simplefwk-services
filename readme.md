#Simple framework - Services

``simplefwk-services`` is a small package containing a simple event callback service as well as a lightweight message service that is intended to be used as a message logger. Native C++ interfaces are provided. Python interfaces are also available and automatically generate by [swig][].

##Installation

A makefile is provided for unix-like operating systems (tested on Mac OS X 10.7.5). Requirements: 
* C++11 compliant c++ compiler (e.g. g++ 4.8)
* Python 2.x (e.g. version 2.7.1)
* swig 2.x 

All the above can be obtained through [macports][] for Max OS X systems.

## Usage

# Incident Service

The Incident Service is a simple implementation of the [observer design pattern] [obs]. It is based on an implementation available in a software framework (known as [Gaudi][]) used for high energy physics experiments. 



[swig]: http://swig.org
[macports]: http://www.macports.org/
[obs]: http://en.wikipedia.org/wiki/Observer_pattern
[Gaudi]: http://cern.ch/gaudi
