///
///  DataTable.cpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#include "DataTable.hpp"
#include "sha1.hpp"

void DataTable::applyData(int pid, int ppid, unsigned long long time, bool isOpen, std::string path) {
    procdata data {
        pid,
        ppid,
        time,
        isOpen,
        path
    };
    applyData(data);
}

void DataTable::Add(procdata item) {
    item.sha1_hash = SHA1::from_file(item.path);
    _data.emplace(item.pid, item);
}

void DataTable::Remove(pid_t pid) {
    if (_data.find(pid) != _data.end()) {
        _data.erase(pid);
    }
}

void DataTable::applyData(procdata data) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!data.isOpen) {
        Remove(data.pid);
    }
    else if (data.isOpen) {
        Add(data);
    }
}

