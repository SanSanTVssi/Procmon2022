///
///  DataTableWrapper.m
///  SwiftParserTest
///
///  Created by Aai2002 on 01.02.2022.
///

#import "UIClient.h"
#include "Data/DataTable.hpp"
#include "IPC/PipeMonitor.h"

#include <string>
#include <thread>
#include <chrono>

DataTable table;
decltype(table.begin()) iterator = table.begin();
PipeMonitor pipe_monitor;

@implementation UIClient

+(void) ApplyData   : (int) pid
                    : (int) ppid
                    : (unsigned long long) time
                    : (bool) isBool
                    : (NSString*) path
{
    std::string path_str = std::string([path UTF8String]);
    table.applyData(pid, ppid, time, isBool, path_str);
}

+(void) sleep : (int) time_nanoseconds { std::this_thread::sleep_for(std::chrono::nanoseconds(time_nanoseconds)); }

+(void) PipeMonitorInit { pipe_monitor.init(&table); }

+(void) begin { iterator = table.begin(); }
+(void) end { iterator = table.end()--; }
+(void) increment { iterator++; }
+(void) decrement { iterator--; }

+(int) getPidByIterator { return iterator->second.pid; }

+(int) getPPid: (int) pid { return table[pid].ppid; }
+(unsigned long long) getTime: (int) pid { return table[pid].time; }
+(bool) getIsOpen: (int) pid { return table[pid].isOpen; }
+(NSString*) getPath: (int) pid {
    NSString* to_return = [NSString stringWithUTF8String:table[pid].path.c_str()];
    return to_return;
}
+(NSString*) getHash: (int) pid {
    NSString* to_return = [NSString stringWithUTF8String:table[pid].sha1_hash.c_str()];
    return to_return;
}
+(NSString*) getName: (int) pid {
    NSString* to_return = [NSString stringWithUTF8String:table[pid].name.c_str()];
    return to_return;
}

+(unsigned long) size {
    return table.get().size();
}

+(bool) isEnd { return iterator == table.end(); }
+(bool) isBegin { return iterator == table.begin(); }

@end
