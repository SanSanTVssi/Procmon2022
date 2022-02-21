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
#include "procdata.h"

class Packet
{
private:
    std::string buff;
    unsigned long size;
    template <typename T>
    std::string GetBytes(T data)
    {
        char* bytePointer = reinterpret_cast<char*>(&data);
        std::string toReturn="";
        for(int i=0; i< sizeof(T); ++i)
        {
            toReturn+= *(bytePointer+i);
        }
        return toReturn;
    }
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
    }
    void Write(int data)
    {
        buff += GetBytes(data);
    }
    void Write(bool data)
    {
        buff += GetBytes(data);
    }
    void Write(unsigned long long data)
    {
        buff += GetBytes(data);
    }
    void Write(std::string data)
    {
        int len = data.size();
        Write(len);
        buff += data;
    }
    void WriteLength()
    {
        int len = buff.size();
        std::string newBuff= GetBytes(len);
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
    int ReadBool()
    {
        bool toReturn;
        char boolBuff[sizeof(bool)];
        for(int i=0; i < sizeof(bool);++i)
        {
            boolBuff[i] = buff[0];
            buff.erase(buff.begin());
        }
        std::memcpy(&toReturn, boolBuff, sizeof(bool));
        return toReturn;
    }
    unsigned long ReadULLong()
    {
        unsigned long long toReturn;
        char ulongBuff[sizeof(unsigned long long)];
        for(int i=0; i< sizeof(unsigned long long); ++i)
        {
            ulongBuff[i] = buff[0];
            buff.erase(buff.begin());
        }
        std::memcpy(&toReturn, ulongBuff, sizeof(unsigned long long));
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
    procdata ReadProcdata() {
        procdata to_return;
        to_return.pid = ReadInt();
        to_return.ppid = ReadInt();
        to_return.time = ReadULLong();
        to_return.isOpen = ReadBool();
        to_return.path = ReadString().c_str();
        to_return.name = ReadString().c_str();
        return to_return;
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
};
#endif /* Packet_h */
