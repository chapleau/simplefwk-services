#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>


template <typename T>
class Singleton
{


protected:
  // Constructeur/destructeur
  Singleton () { }
 virtual ~Singleton () { }
  

public:
  // Interface publique
  static T *getInstance ()
  {
    if (NULL == _singleton)
      {
        //std::cout << "creating singleton. base" << std::endl;
        _singleton = new T;
        _singleton->post_init();
      }
    //else
    //  {
    //     std::cout << "singleton already created! "<<_singleton->name() << std::endl;
    //   }

    return (static_cast<T*> (_singleton));
  }
  
  static bool isAlive() { return _singleton != NULL; }

  static void kill ()
  {
    if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
  }

private:
  // Unique instance
  static T *_singleton;
};

template <typename T>
T *Singleton<T>::_singleton = NULL;

#endif
