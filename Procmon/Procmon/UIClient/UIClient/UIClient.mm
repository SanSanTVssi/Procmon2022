///
///  DataTableWrapper.m
///  SwiftParserTest
///
///  Created by Aai2002 on 01.02.2022.
///

#import "UIClient.h"
#include "DataStorages/DataTable.hpp"
#include "IPC/PipeMonitor.h"

#include <string>
#include <thread>
#include <chrono>

DataTable table;
decltype(table.Begin()) iterator = table.Begin();
PipeMonitor pipe_monitor;

@implementation UIClient

+(void) applyData   : (int) pid
                    : (int) ppid
                    : (unsigned long long) time
                    : (bool) isBool
                    : (NSString*) path
{
    std::string path_str = std::string([path UTF8String]);
    table.ApplyData(pid, ppid, time, isBool, path_str);
}

+(void) pipeMonitorInit { pipe_monitor.init(&table); }

+(void) begin { iterator = table.Begin(); }
+(void) end { iterator = table.End()--; }
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

+(NSString*) getTimeStr: (int) pid {
    NSString* to_return = [NSString stringWithUTF8String:table[pid].time_str.c_str()];
    return to_return;
}

+(unsigned long) size {
    return table.Get().size();
}

+(bool) isEnd { return iterator == table.End(); }
+(bool) isBegin { return iterator == table.Begin(); }

@end
