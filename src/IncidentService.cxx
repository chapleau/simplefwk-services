#include "Services/IIncidentListener.h"
#include "Services/Incident.h"
#include "Services/IncidentService.h"

#include "Services/INamed.h"



IncidentService::IncidentService() : Messaging("IncidentService") {}

IncidentService::~IncidentService() {
  //cleanup
  for (auto & list: m_listenerMap) delete list.second;
}


void IncidentService::post_init() {

  LOG("in post_init", logINFO);

}

void IncidentService::addListener(IIncidentListener* lis, const std::string& type, int priority) {


   ListenerMap::iterator itMap = m_listenerMap.find(type);
   if( itMap == m_listenerMap.end() ) {
   
      //first time we encounter this type of incident
      ListenerList* newlist = new ListenerList();
      std::pair<ListenerMap::iterator, bool> p = m_listenerMap.insert(ListenerMap::value_type(type, newlist));
      if( p.second ) itMap = p.first;
   
   }
   
   ListenerList* llist = (*itMap).second;
   
   ListenerList::iterator itlist;
   for( itlist = llist->begin(); itlist != llist->end(); itlist++ ) {
   
       if( (*itlist).priority < priority ) break;
   
   }
   
   llist->insert(itlist,Listener(lis, priority));
   
   const INamed * named = dynamic_cast<const INamed*>(lis);
   std::string name = (named) ? named->name() : "unknown";
   
   
   LOG("Added listener ("+name+") for type "+type+" with priority "<<priority, logINFO);


}

//no protection against calling removeListener during a fireIncident..for now.
void IncidentService::removeListener(IIncidentListener* lis, const std::string& type) {


    ListenerMap::iterator itmap = m_listenerMap.find( type );
    if( itmap == m_listenerMap.end() ) return;
    
    ListenerList* llist = (*itmap).second;
    ListenerList::iterator itlist;
    
    for( itlist = llist->begin(); itlist != llist->end();)
       if ( (*itlist).iListener == lis || lis == 0) itlist = llist->erase(itlist); // remove from the list now
       else itlist++;
    
    
    if( llist->size() == 0) {
         delete llist;
         m_listenerMap.erase(itmap);
    }

}

void IncidentService::fireIncident (const Incident& incident) {

   LOG("Fireing "+incident.svcType(), logINFO);

   ListenerMap::iterator itmap = m_listenerMap.find(incident.svcType());
   if ( m_listenerMap.end() == itmap ) return;
   
   
   ListenerList* llist = (*itmap).second;
   for( auto & listener: *llist) {
   
      try { listener.iListener->handle(incident); }
      catch( const std::exception& exc ) {
         LOG("std::exception during "<<incident.svcType()<<" : "<<exc.what(), logERROR);
      }
      catch(...) {
         LOG("unknown exception during "<<incident.svcType(), logERROR);
      }
   
   }

}

















