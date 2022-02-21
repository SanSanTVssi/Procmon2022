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
public:
    
    /// <summary>
    /// This method is needed to add new data to the table or delete
    /// depending on whether the process opened or closed
    /// </summary>
    /// <param name="data">dato for save to table</param>
    void ApplyData(procdata data);
    void ApplyData(int pid, int ppid, unsigned long long time, bool isOpen, std::string path);
    
    /// <summary>
    /// Get acces to read data in table
    /// </summary>
    /// <returns>const link to data</returns>
    const auto& Get() const { return data; }
    
    auto Begin() { return data.begin(); }
    
    auto End() { return data.end(); }
    
    procdata operator[] (int pid);
    
    auto& GetMutex() { return mutex; }
    
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
    
    std::string uTimeToStr(unsigned long long utime);
};


#endif /* DataTable_hpp */
