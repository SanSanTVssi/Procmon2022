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
#include <map>
#include <thread>
#include "DataTable.hpp"

struct procdata {
    int pid;
    int ppid;
    unsigned long long time;
    bool isOpen;
    std::string path;
    std::string name;
};

class DataTable {
private:
    std::map<pid_t, procdata> data;
    std::size_t size;
    std::mutex mutex;
    
    /// <summary>
    /// Add new data to table
    /// </summary>
    void add(procdata);
    
    /// <summary>
    /// Remove data from table
    /// </summary>
    void remove(pid_t);
    
public:
    
    /// <summary>
    /// This method is needed to add new data to the table or delete
    /// depending on whether the process opened or closed
    /// </summary>
    /// <param name="data">dato for save to table</param>
    void ApplyData(procdata pdata);
    
    /// <summary>
    /// Get acces to read data in table
    /// </summary>
    /// <returns>const link to data</returns>
    const auto& get() const { return data; }
    
    auto& getMutex() { return mutex; }
};



#endif /* DataTable_hpp */
