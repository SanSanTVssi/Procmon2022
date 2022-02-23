///
///  PipeMonitor.cpp
///  Daemon
///
///  Created by Ааи on 15.02.2022.
///

#include "PipeMonitor.h"

namespace Emmiter {

void UIConnection::Send(Packet packet) {
    {
        std::unique_lock<std::mutex> lock(mut);
        write(fd,packet.GetBuffer(), 256);
        if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        { perror("signal"); }
    }
}

PipeMonitor::~PipeMonitor() {
    close(perm_pipe_fd);
}

void PipeMonitor::createUIConnection(std::string path) {
    umask(0);
    mkfifo(path.c_str(), 0666);
    UIConnection connection(open(path.c_str(), O_WRONLY));
    sendTable(connection);
    connections.push_back(std::move(connection));
}

void PipeMonitor::sendTable(UIConnection& client) {
    std::unique_lock<std::mutex> lock(table->getMutex());
    auto data = table->get();
    for (const auto& [key, value]: data) {
        Packet packet;
        packet.Write(value);
        packet.WriteLength();
        client.Send(packet);
    }
}

void PipeMonitor::startListening() {
    std::string perm_fifo_path = "/tmp/common_pipe_procmon";
    umask(0);
    mkfifo(perm_fifo_path.c_str(), 0666);
    
    while(true) {
        perm_pipe_fd = open(perm_fifo_path.c_str(), O_RDONLY);
        char buff[80];
        read(perm_pipe_fd, buff, 80);
        Packet packet(buff);
        std::string path=packet.ReadString();
        std::thread th(&PipeMonitor::createUIConnection,this, path);
        th.detach();
        
        close(perm_pipe_fd);
    }
}

void PipeMonitor::init(DataTable * Table) {
    table = Table;
    std::async(std::launch::async,
               &PipeMonitor::startListening, this);
    
    
}

void PipeMonitor::SendUpdates(Packet packet) {
    std::lock_guard<std::mutex> lock(mut);
    to_send_list.push_back(packet);
}

void PipeMonitor::sendLoop() {
    while(true) {
        std::lock_guard<std::mutex> lock(mut);
        for (Packet packet: to_send_list) {
            for (auto& connection: connections) {
                connection.Send(packet);
            }
        }
        to_send_list.clear();
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}
}
