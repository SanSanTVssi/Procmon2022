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

struct procdata {
    pid_t pid;
    pid_t ppid;
    uint64_t time;
    bool isOpen;
    std::string path;
    
    friend bool operator==(const procdata& instance1,  const procdata& instance2) {
        return instance1.pid == instance2.pid && instance1.ppid == instance2.ppid && instance1.time == instance2.time && instance1.path == instance2.path && instance1.isOpen == instance2.isOpen;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const procdata& instance) {
        out << "pid:\t"    << instance.pid    << "\t"
            << "ppid:\t"   << instance.ppid   << "\t"
            << "time:\t"   << instance.time   << "\t"
            << "isOpen:\t" << instance.isOpen << "\t"
            << "path:\t"   << instance.path   << "\t"
        ;
        return out;
    }
};

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
    
    /// <summary>
    /// Need for logging and debugging
    /// Print data from table
    /// </summary>
    void WriteTable();
    
    /// <summary>
    /// Get acces to read data in table
    /// </summary>
    /// <returns>const link to data</returns>
    const auto& get() const { return _data; }
    
    auto& getMutex() { return mutex; }
};



#endif /* DataTable_hpp */
