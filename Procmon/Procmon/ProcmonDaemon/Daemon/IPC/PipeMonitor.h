///
///  PipeMonitor.h
///  PipeSetupForDaemon
///
///  Copyright © 2022 Maksym. All rights reserved.
///

#ifndef PipeMonitor_h
#define PipeMonitor_h
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <future>
#include <vector>
#include <list>
#include <queue>
#include "../DataStorages/Packet.h"
#include "../ProcessMonitor/Monitor.hpp"
#include "../DataStorages/DataTable.hpp"

namespace Emmiter {

class UIConnection {
    int fd;
    std::mutex mut;
public:
    
    UIConnection(UIConnection &other) { fd = other.fd; }
    
    UIConnection(UIConnection &&other) { fd = std::move(other.fd); }
    
    UIConnection(int fd_) : fd(fd_) {}
    
    void Send(Packet packet);
    
};

class PipeMonitor {
public:
    PipeMonitor() : table(nullptr), send_thread([this](){ this->sendLoop(); }) { }
    
    ~PipeMonitor();
    
    void init(DataTable * Table);
    
    void SendUpdates(Packet packet);
    
private:
    friend class UIConnection;
    DataTable * table;
    std::thread send_thread;
    std::list<Packet> to_send_list;
    std::mutex mut;
    int perm_pipe_fd;
    
    void sendTable(UIConnection& client);
    
    std::vector<UIConnection> connections;
    
    void createUIConnection(std::string path);
    
    void startListening();
    
    void sendLoop();
};
}
#endif /* PipeMonitor_h */
