///
///  DataTableWrapper.h
///  SwiftParserTest
///
///  Created by Aai2002 on 01.02.2022.
///

#ifndef DataTableWrapper_h
#define DataTableWrapper_h

#import <Foundation/Foundation.h>
#include "stdio.h"

void ConsoleClear() { system("TERM=xterm-256color;clear"); }

@interface UIClient : NSObject

+(void) ApplyData   : (int) pid
                    : (int) ppid
                    : (unsigned long long) time
                    : (bool) isBool
                    : (NSString*) path;

+(void) sleep : (int) time_nanoseconds;

+(void) PipeMonitorInit;

+(void) begin;
+(void) end;
+(void) increment;
+(void) decrement;

+(int) getPidByIterator;

+(int) getPPid: (int) pid;
+(unsigned long long) getTime: (int) pid;
+(bool) getIsOpen: (int) pid;
+(NSString*) getPath: (int) pid;
+(NSString*) getHash: (int) pid;
+(NSString*) getName: (int) pid;

+(unsigned long) size;

+(bool) isEnd;
+(bool) isBegin;

@end

#endif /* DataTableWrapper_h */

