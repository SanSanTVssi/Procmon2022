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
#include <vector>
#include <ctime>
#include <random>
#include "../Data/Packet.h"
#include "../Data/DataTable.hpp"


class PipeMonitor
{
private:
    std::string CreateUIClientName() {
        time_t seconds = time(NULL);
        std::random_device rd;
        std::mt19937 mersenne(rd());
        int rand = mersenne();
        std::string new_pipe_name = "pipe" + std::to_string(seconds + rand);
        return new_pipe_name;
    }
    DataTable * table;
    void InitiateConnection()
    {
        int fd=-1;
        std::string path= "/tmp/common_pipe_procmon";
        mkfifo(path.c_str(), 0666);
        while(fd<0)
        {
            fd = open(path.c_str(), O_WRONLY);
            std::cout << strerror(errno) << std::endl;
        }
        std::string new_path = "/tmp/" + CreateUIClientName();
        Packet packet;
        packet.Write(new_path);
        packet.WriteLength();
        write(fd,packet.GetBuffer(), 80);
        close(fd);
        std::cout<<"Succes!!"<<std::endl;
        std::thread thread(&PipeMonitor::StartReading, this, new_path);
        thread.detach();
        //std::async(std::launch::async,&PipeMonitor::StartReading, this, new_path);
    }
    void StartReading(std::string path)
    {
        int fd;
        ssize_t bytesReceived;
        mkfifo(path.c_str(), 0666);
        fd = open(path.c_str(), O_RDONLY);
        char* buff=new char[256];
        while(true)
        {
            bytesReceived=read(fd, buff, 256);
            if(bytesReceived>0)
            {
                Packet packet(buff);
                procdata output = packet.ReadProcdata();
                table->applyData(output);
            }
            if(bytesReceived<0)
            {
                std::cout<<strerror(errno)<<std::endl;
            }
        }
    }
public:
    PipeMonitor() : table(nullptr) {}
    
    void init(DataTable * Table) {
        table = Table;
        std::thread thread(&PipeMonitor::InitiateConnection, this);
        thread.detach();
        //std::async(std::launch::async, &PipeMonitor::InitiateConnection, this);
    }
};

#endif /* PipeMonitorUI_h */
