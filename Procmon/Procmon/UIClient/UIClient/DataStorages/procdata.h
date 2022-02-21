///
///  procdata.h
///  SwiftParserTest
///
///  Created by Aai2002 on 01.02.2022.
///

#ifndef procdata_h
#define procdata_h

#include <string>

typedef struct {
    int pid;
    int ppid;
    unsigned long long time;
    int isOpen;
    std::string path;
    std::string sha1_hash;
    std::string name;
    std::string time_str;
} procdata;

#endif /* procdata_h */
