///
///  Daemon.hpp
///  Daemon
///  In this file start working process monitoring
///  and client observing.
///  For process monitoring was used Endpoint Security Framework (ESF)
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  31.01.2022.
///

#ifndef Daemon_hpp
#define Daemon_hpp

#include <stdexcept>
#include <iostream>
#include <thread>
#include <chrono>
#include "ProcessMonitor/Monitor.hpp"
#include "DataStorages/DataTable.hpp"
#include "IPC/PipeMonitor.h"

class ProcmonDaemon {
public:
    
    /// <summary>
    /// initialize client observing and process monitoring
    /// </summary>
    static void init();
    
private:
    static DataTable buffer;
    static Emmiter::PipeMonitor client_observer;
    static Monitor process_monitor;
    
    /// <summary>
    /// Function for processing ESF events
    /// </summary>
    /// <param name="client">ESF client</param>
    /// <param name="msg">message from client (store info about client)</param>
    static void handle_event(es_client_t *client, const es_message_t *msg);
};

#endif /* Daemon_hpp */
