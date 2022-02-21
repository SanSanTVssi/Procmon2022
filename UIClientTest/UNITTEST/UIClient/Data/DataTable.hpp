///
///  DataTable.hpp
///  DaemonESTest
///
///  Copyright © 2022 Aai2002. All rights reserved.
///  25.01.2022.
///

#ifndef DataTable_hpp
#define DataTable_hpp

#include <stdexcept>
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include "procdata.h"

class DataTable {
private:
    std::map<pid_t, procdata> _data;
    std::size_t _size;
    std::mutex mutex;
    
    /// <summary>
    /// Add new data to table
    /// </summary>
    void Add(procdata);
    
    /// <summary>
    /// Remove data from table
    /// </summary>
    void Remove(pid_t);
    
public:
    
    /// <summary>
    /// This method is needed to add new data to the table or delete
    /// depending on whether the process opened or closed
    /// </summary>
    /// <param name="data">dato for save to table</param>
    void applyData(procdata data);
    void applyData(int pid, int ppid, unsigned long long time, bool isOpen, std::string path);
    
    /// <summary>
    /// Get acces to read data in table
    /// </summary>
    /// <returns>const link to data</returns>
    const auto& get() const { return _data; }
    
    auto begin() { return _data.begin(); }
    
    auto end() { return _data.end(); }
    
    auto operator[] (int pid) { return _data[pid]; }
    
    auto& getMutex() { return mutex; }
};


#endif /* DataTable_hpp */
