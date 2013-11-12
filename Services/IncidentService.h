#include <unordered_map>
#include <list>

#include "Services/singleton.h"
#include "Services/Messaging.h"


class IIncidentListener;
class Incident;

class IncidentService : public Singleton<IncidentService>, public Messaging {

friend class Singleton<IncidentService>;



public:

   void addListener(IIncidentListener*, const std::string&, int priority = 0, bool single = false);
   void removeListener(IIncidentListener*, const std::string&);


   void fireIncident (const Incident& );
   
   
protected:
   
   void post_init();
   
   
private:

   struct Listener
   {
     IIncidentListener* iListener;
     int priority;
     bool singleShot;
     
     Listener(IIncidentListener* il, int pri, bool single=false) : iListener(il), priority(pri), singleShot(single) {}
   };
   
   // Typedefs
   typedef std::list<Listener> ListenerList;
   typedef std::unordered_map<std::string, ListenerList*> ListenerMap;

   ListenerMap  m_listenerMap;
   
   const std::string *m_currentIncidentType;
   
   
   /// Helper class to identify a Listener that have to be removed from a list.
    struct listenerToBeRemoved{
        inline bool operator() (const Listener& l) {
            return l.singleShot;
        }
    };
    

   
     IncidentService();
    ~IncidentService();

};
