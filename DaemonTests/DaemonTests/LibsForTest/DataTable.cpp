///
///  DataTable.cpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#include "DataTable.hpp"

void DataTable::Add(procdata item) {
    _data.emplace(item.pid, item);
}

void DataTable::Remove(pid_t pid) {
    _data.erase(pid);
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
void DataTable::WriteTable() {
    for (const auto& [key, value] : _data) {
        std::cout << value << std::endl;
    }
}

