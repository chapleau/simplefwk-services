#include <unordered_map>
#include <list>

#include "Services/singleton.h"
#include "Services/Messaging.h"


class IIncidentListener;
class Incident;

class IncidentService : public Singleton<IncidentService>, public Messaging {

friend class Singleton<IncidentService>;



public:

   void addListener(IIncidentListener*, const std::string&, int priority = 0);
   void removeListener(IIncidentListener*, const std::string&);


   void fireIncident (const Incident& );
   
   
protected:
   
   void post_init();
   
   
private:

   struct Listener
   {
     IIncidentListener* iListener;
     int priority;
     
     Listener(IIncidentListener* il, int pri) : iListener(il), priority(pri) {}
   };
   
   // Typedefs
   typedef std::list<Listener> ListenerList;
   typedef std::unordered_map<std::string, ListenerList*> ListenerMap;

   ListenerMap  m_listenerMap;
   
   

   
     IncidentService();
    ~IncidentService();

};
