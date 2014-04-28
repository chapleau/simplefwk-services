#ifndef NAMED_H
#define NAMED_H

#include <string>


class INamed {


public :

 virtual const std::string&  name() const = 0;
 virtual void setname(const std::string&) = 0;

//cannot delete from instance of INamed
protected :

 virtual ~INamed() {}


};

#endif