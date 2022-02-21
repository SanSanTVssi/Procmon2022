//
//  PipeMonitorUI.h
//  PipeSetupForUI
//
//  Created by Maksym on 01.02.2022.
//  Copyright © 2022 Maksym. All rights reserved.
//

#ifndef PipeMonitorUI_h
#define PipeMonitorUI_h

#include <errno.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <ctime>
#include <random>
#include <list>
#include "../DataStorages/Packet.h"
#include "../DataStorages/DataTable.hpp"


class PipeMonitor
{
public:
    PipeMonitor() : table(nullptr) {}
    
    void init(DataTable * Table) {
        table = Table;
        std::thread thread(&PipeMonitor::initiateConnection, this);
        thread.detach();
    }
    
private:
    std::string createUIClientName() {
        time_t seconds = time(NULL);
        std::random_device rd;
        std::mt19937 mersenne(rd());
        int rand = mersenne();
        std::string new_pipe_name = "pipe" + std::to_string(seconds + rand);
        return new_pipe_name;
    }
    DataTable * table;
    void initiateConnection()
    {
        int fd=-1;
        std::string path= "/tmp/common_pipe_procmon";
        int counter = 100000;
        while(fd < 0 && --counter)
        {
            fd = open(path.c_str(), O_WRONLY);
            std::cout << strerror(errno) << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(300));
        }
        std::string new_path = "/tmp/" + createUIClientName();
        Packet packet;
        packet.Write(new_path);
        packet.WriteLength();
        write(fd,packet.GetBuffer(), 80);
        close(fd);
        std::cout<<"Succes!!"<<std::endl;
        startReading(new_path);
    }
    void startReading(std::string path)
    {
        int fd = -1;
        ssize_t bytesReceived;
        int count = 100000;
        while (fd < 0 && --count) {
            fd = open(path.c_str(), O_RDONLY);
            std::cout << strerror(errno) << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(300));
        }
        char* buff=new char[256];
        while(true)
        {
            bytesReceived=read(fd, buff, 256);
            if(bytesReceived>0)
            {
                Packet packet(buff);
                procdata output = packet.ReadProcdata();
                table->ApplyData(output);
            }
            if(bytesReceived<0)
            {
                std::cout<<strerror(errno)<<std::endl;
            }
        }
    }
};

#endif /* PipeMonitorUI_h */
