//inspiration : Gaudi's IncidentSvc !

#include "Services/IIncidentListener.h"
#include "Services/Incident.h"
#include "Services/IncidentService.h"

#include "Services/INamed.h"



IncidentService::IncidentService() : Messaging("IncidentService"), m_currentIncidentType(0) {}

IncidentService::~IncidentService() {
  //cleanup
  for (auto & list: m_listenerMap) delete list.second;
}


void IncidentService::post_init() {

  LOG("in post_init", logDEBUG);

}

void IncidentService::addListener(IIncidentListener* lis, const std::string& type, int priority, bool single) {


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
   
   llist->insert(itlist,Listener(lis, priority, single));
   
   const INamed * named = dynamic_cast<const INamed*>(lis);
   std::string name = (named) ? named->name() : "unknown";
   
   
   LOG("Added listener ("<<name<<") for type "<<type<<" with priority "<<priority, logDEBUG);


}


void IncidentService::removeListener(IIncidentListener* lis, const std::string& type) {
    
    
    ListenerMap::iterator itmap = m_listenerMap.find( type );
    if( itmap == m_listenerMap.end() ) return;
    
    
    const INamed * named = dynamic_cast<const INamed*>(lis);
    std::string name = (named) ? named->name() : "unknown";
    
    ListenerList* llist = (*itmap).second;
    ListenerList::iterator itlist;
    
    bool justScheduleForRemoval = ( 0 != m_currentIncidentType ) && (type == *m_currentIncidentType);
    
    for( itlist = llist->begin(); itlist != llist->end();)
        if ( (*itlist).iListener == lis || lis == 0) {
            if (justScheduleForRemoval) {
                (itlist++)->singleShot = true; // remove it as soon as it is safe
                LOG("to be removed: listener ("<<name<<") for type "<<type, logDEBUG);
            }
            else {
                itlist = llist->erase(itlist); // remove from the list now
                LOG("Removed listener ("<<name<<") for type "<<type, logDEBUG);
            }
        }
        else itlist++;
    
    
    if( llist->size() == 0) {
        delete llist;
        m_listenerMap.erase(itmap);
    }
    
}



void IncidentService::fireIncident (const Incident& incident) {

   LOG("Fireing "+incident.svcType(), logDEBUG);

   ListenerMap::iterator itmap = m_listenerMap.find(incident.svcType());
   if ( m_listenerMap.end() == itmap ) return;
   
   
   //set the current incident so that we don't remove listener now
   m_currentIncidentType = &(incident.svcType());
   bool weHaveToCleanUp = false;
   
   ListenerList* llist = (*itmap).second;
   for( auto & listener: *llist) {
   
      try { listener.iListener->handle(incident); }
      catch( const std::exception& exc ) {
         LOG("std::exception during "<<incident.svcType()<<" : "<<exc.what(), logERROR);
      }
      catch(...) {
         LOG("unknown exception during "<<incident.svcType(), logERROR);
      }
   
      //keep track of those listeneres that requested to be removed from list
      weHaveToCleanUp |= listener.singleShot;
   }
   
    //perform clean up if necessary
    if (weHaveToCleanUp) {
        // remove all the listeners that need to be removed from the list
        llist->remove_if( listenerToBeRemoved() );
        // if the list is empty, we can remove it
        if( llist->size() == 0) {
            delete llist;
            m_listenerMap.erase(itmap);
        }
    }
   
   m_currentIncidentType = 0;

}

















