///
///  Daemon.cpp
///  Daemon
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  31.01.2022.
///

#include "Daemon.hpp"

#include "unistd.h"

Emmiter::PipeMonitor ProcmonDaemon::client_observer;
Monitor ProcmonDaemon::process_monitor;
DataTable ProcmonDaemon::buffer;

void ProcmonDaemon::handle_event(es_client_t *client, const es_message_t *msg) {
    Packet data_packet;
    int pid = audit_token_to_pid(msg->process->audit_token);
    procdata data {
        pid,
        audit_token_to_pid(msg->process->parent_audit_token),
        msg->mach_time,
        false,
        PreMonitor::PathByPid(pid),
        PreMonitor::NameByPid(pid)
    };
    
    if (msg->event_type == ES_EVENT_TYPE_NOTIFY_EXEC) {
        data.isOpen = true;
    }
    data_packet.Write(data);
    data_packet.WriteLength();
    client_observer.SendUpdates(data_packet);
    buffer.ApplyData(data);
}

void ProcmonDaemon::init() {
    PreMonitor::SetListOfAlreadyRunningProcesses(buffer);
    process_monitor.init(handle_event);
    process_monitor();
    client_observer.init(&buffer);
}
