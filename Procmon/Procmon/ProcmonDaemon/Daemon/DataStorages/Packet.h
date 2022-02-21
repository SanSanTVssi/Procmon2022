///
///  Packet.h
///  PipeTest
///
///  Created by Maksym on 21.01.2022.
///  Copyright © 2022 Maksym. All rights reserved.
///

#ifndef Packet_h
#define Packet_h
#include <string>
#include "../DataStorages/DataTable.hpp"
class Packet {
public:
    Packet() : buff(), size(0) {}
    Packet(char* buffer) : size(0)
    {
        int len;
        char intbuf[4];
        for(int i=0; i< sizeof(int); ++i)
        {
            intbuf[i] = buffer[i];
        }
        memcpy(&len, intbuf,4);
        if(len<4) return;
        for(int i=4; i<len+4; ++i)
        {
            buff += buffer[i];
        }
    }
    void Write(const procdata& data) {
        Write(static_cast<int>(data.pid));
        Write(static_cast<int>(data.ppid));
        Write(static_cast<unsigned long long>(data.time));
        Write(data.isOpen);
        Write(data.path);
        Write(data.name);
    }
    void Write(int data)
    {
        buff += getBytes(data);
    }
    void Write(bool data)
    {
        buff += getBytes(data);
    }
    void Write(unsigned long long data)
    {
        buff += getBytes(data);
    }
    void Write(std::string data)
    {
        int len = static_cast<int>(data.size());
        Write(len);
        buff += data;
    }
    void WriteLength()
    {
        int len = static_cast<int>(buff.size());
        std::string newBuff= getBytes(len);
        newBuff += buff;
        buff = std::move(newBuff);
    }
    int ReadInt()
    {
        int toReturn;
        char intBuff[4];
        for(int i=0; i<sizeof(int);++i)
        {
            intBuff[i] = buff[0];
            buff.erase(buff.begin());
        }
        std::memcpy(&toReturn, intBuff, sizeof(int));
        return toReturn;
    }
    std::string ReadString()
    {
        std::string toReturn="";
        int length = ReadInt();
        for(int i=0; i<length; ++i)
        {
            toReturn += buff[0];
            buff.erase(buff.begin());
        }
        return toReturn;
    }
    const char* GetBuffer()
    {
        return buff.c_str();
    }
    unsigned long GetLength()
    {
        return buff.size();
    }
    void Clear()
    {
        buff.clear();
        size = 0;
    }
    
private:
    std::string buff;
    unsigned long size;
    template <typename T>
    std::string getBytes(T data)
    {
        char* bytePointer = reinterpret_cast<char*>(&data);
        std::string toReturn="";
        for(int i=0; i< sizeof(T); ++i)
        {
            toReturn+= *(bytePointer+i);
        }
        return toReturn;
    }
};
#endif /* Packet_h */
