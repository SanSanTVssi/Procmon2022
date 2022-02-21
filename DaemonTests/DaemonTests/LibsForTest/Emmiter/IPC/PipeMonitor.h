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
#include "../../Packet.h"

namespace Emmiter {
class PipeMonitor
{
private:
    DataTable * table;
    class UIConnection
    {
    public:
        int fd;
        //std::list<Packet> toSend;
        std::mutex mut;
        std::condition_variable condition;
        
        UIConnection(UIConnection &other)
        {
            fd = other.fd;
        }
        UIConnection(UIConnection &&other)
        {
            fd =std::move(other.fd);
        }
        UIConnection(int fd_) : fd(fd_) {}
        void Send(Packet packet)
        {
            std::unique_lock<std::mutex> lock(mut);
            write(fd,packet.GetBuffer(), 256);
            if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
                perror("signal");
        }
    };
    
    void SendTable(UIConnection& client)
    {
        std::unique_lock<std::mutex> lock(table->getMutex());
        auto data = table->get();
        for (const auto& [key, value]: data) {
            Packet packet;
            packet.Write(value);
            packet.WriteLength();
            client.Send(packet);
        }
    }
    std::vector<UIConnection> connections;
    void CreateUIConnection(std::string path)
    {
        mkfifo(path.c_str(), 0666);
        UIConnection connection(open(path.c_str(), O_WRONLY));
        SendTable(connection);
        connections.push_back(std::move(connection));
    }
    void StartListening()
    {
        int perm_pipe_fd;
        std::string perm_fifo_path = "/tmp/common_pipe_procmon";
        mkfifo(perm_fifo_path.c_str(), 0666);
        
        ///> For Tests
        
        isConnected = true;
        
        ///<
        
        while(true)
        {
            perm_pipe_fd = open(perm_fifo_path.c_str(), O_RDONLY);
            char buff[80];
            read(perm_pipe_fd, buff, 80);
            Packet packet(buff);
            std::string path=packet.ReadString();
            std::cout<<path<<std::endl;
            
            std::async(&PipeMonitor::CreateUIConnection,this, path);
            
            close(perm_pipe_fd);
        }
    }
    
public:
    ///> For Tests
    
    bool isConnected = false;
    
    ///<
    
    PipeMonitor() : table(nullptr) { }
    
    void init(DataTable * Table) {
        table = Table;
        std::thread th(
                   &PipeMonitor::StartListening, this);
        th.detach();
    }
    
    void SendUpdates(Packet packet)
    {
        ///
        ///Writing data to packet
        ///
        ///
        ///packet.WriteLength();
        ///Dont forget to write length of
        ///packet in the begining
        for(int i=0; i<connections.size(); ++i)
        {
            std::thread th(&PipeMonitor::UIConnection::Send,&connections[i], packet);
            th.detach();
        }
    }
};
}
//PipeMonitor::perm_pipe_fd = 0;
#endif /* PipeMonitor_h */
