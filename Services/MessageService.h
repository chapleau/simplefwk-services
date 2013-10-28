#include <sstream>
#include <memory>
 
#include "Services/log.h"
#include "Services/IIncidentListener.h"
#include "Services/IncidentService.h"

#include "Services/singleton.h"
#include "Services/Messaging.h"

#include <unordered_map>
#include <map>


/////////////////////////////////////////////////////////////////////////////
//
// Message Service
//
/////////////////////////////////////////////////////////////////////////////

class MessageService : public Singleton<MessageService>, public Messaging, virtual public IIncidentListener
{
  friend class Singleton<MessageService>;

private:
  // Constructeur/destructeur
  MessageService (): Messaging("MessageService") { }
  
  ~MessageService () {}

protected:

  void post_init() {
     
     //retrieve incident service:
    LOG("in post_init", logINFO);
    IncidentService * inc_svc = IncidentService::getInstance();
    if (!inc_svc) return;
    inc_svc->addListener(this, "EndRun", -1);
  
  }

public:
  // Interface publique
  std::shared_ptr<FILELog> MSG(TLogLevel level, const INamed* n) {
   
         //count
         auto & count_ = m_msg_count[n->name()];
         auto & count  = count_[level];
         count++;
      
         return std::shared_ptr<FILELog>(new FILELog(level));
     
  }
  
  //void SetReportingLevel(TLogLevel level) {m_lvl = level;}
  //bool ReportingLevel(TLogLevel level) {return !(level > m_lvl || !Output2FILE::Stream()) ;}
  //void SetReportingLevel(std::string str_level) {m_lvl = FILELog::FromString(str_level);}
  
  
  void handle(const Incident& inc) {
  
     if ( inc.svcType() != IncidentType::EndProcessing ) return;
  
     std::ostringstream oss(std::ios_base::app);
     oss<<"\n---\n Summary: ";
     for (auto const & names : m_msg_count) {
         oss<<"\n    "<<names.first<<" : ";
         for (auto const & levels : names.second)
            oss<< FILELog::ToString(levels.first)<<" ("<< levels.second<<")  ";
        
     }
     oss<<"\n---";
  
     if ( dolog(logINFO) ) Messaging::MSG(oss, logINFO);

     
  
  
     //commit suicide
     this->kill();
      
  }
  
 
  
  
private:
  //TLogLevel m_lvl;
  
  std::unordered_map<std::string, std::map<TLogLevel, unsigned int> > m_msg_count;
  
  
  
  
  
};





