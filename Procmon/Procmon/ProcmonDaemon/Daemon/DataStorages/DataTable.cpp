///
///  DataTable.cpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#include "DataTable.hpp"

void DataTable::add(procdata item) {
    data.emplace(item.pid, item);
}

void DataTable::remove(pid_t pid) {
    data.erase(pid);
}

void DataTable::ApplyData(procdata pdata) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!pdata.isOpen) {
        remove(pdata.pid);
    }
    else if (pdata.isOpen) {
        add(pdata);
    }
}

