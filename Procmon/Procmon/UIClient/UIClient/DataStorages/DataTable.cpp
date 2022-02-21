///
///  DataTable.cpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#include "DataTable.hpp"
#include "sha1.hpp"

#include <chrono>
#include <ctime>

#include <errno.h>
#include <libproc.h>

void DataTable::ApplyData(int pid, int ppid, unsigned long long time, bool isOpen, std::string path) {
    procdata data {
        pid,
        ppid,
        time,
        isOpen,
        path
    };
    ApplyData(data);
}

procdata DataTable::operator[] (int pid) {
    procdata data;
    data.pid = 0;
    data.ppid = 0;
    data.time = 0;
    data.path = "";
    data.isOpen = false;
    data.name = "";
    data.time_str = "";
    if (this->data.find(pid) != this->data.end()) {
        data = this->data[pid];
    }
    return data;
}

std::string DataTable::uTimeToStr(unsigned long long utime) {
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string timeStr = std::string(std::ctime(&currentTime));
    timeStr.pop_back();
    return timeStr;
}

void DataTable::add(procdata item) {
    item.sha1_hash = SHA1::from_file(item.path);
    item.time_str = uTimeToStr(item.time);
    data.emplace(item.pid, item);
}

void DataTable::remove(pid_t pid) {
    if (data.find(pid) != data.end()) {
        data.erase(pid);
    }
}

void DataTable::ApplyData(procdata data) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!data.isOpen) {
        remove(data.pid);
    }
    else if (data.isOpen) {
        add(data);
    }
}

