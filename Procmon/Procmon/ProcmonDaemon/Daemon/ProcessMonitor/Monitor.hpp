///
///  Monitor.hpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#ifndef Monitor_hpp
#define Monitor_hpp

#include <EndpointSecurity/EndpointSecurity.h>
#include <dispatch/dispatch.h>
#include <bsm/libbsm.h>
#include <functional>
#include <iostream>
#include <string>

#include "../DataStorages/DataTable.hpp"

namespace PreMonitor {
std::string PathByPid(int pid);
std::string NameByPid(int pid);
void SetListOfAlreadyRunningProcesses(DataTable& table);
}

class Monitor {
    std::function<void(es_client_t *client,
                       const es_message_t *msg)> _handle_event;
public:
    
    Monitor() = default;
    
    ~Monitor();
    
    int init(decltype(_handle_event) handle_event,
              unsigned int size = 2,
              es_event_type_t* Events = nullptr);
    
    int operator()();
    
    void operator()(std::function<void()>);
    
private:
    es_client_t *_client;
    es_event_type_t *_events;
    unsigned int _size;
    
    bool _is_inited = false;
    bool _is_subscribed = false;
    
    void _CreateNewESClient();
    
    void _SetDefaultEvents();
    
};

#endif /* Monitor_hpp */
