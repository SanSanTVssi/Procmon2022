//math_test.cpp
#include <gtest/gtest.h>
#include <iostream>
#include "LibsForTest/DataTable.hpp"
#include "LibsForTest/Packet.h"

// IPC tests
#include "LibsForTest/Receiver/IPC/PipeMonitor.h"
#include "LibsForTest/Emmiter/IPC/PipeMonitor.h"

namespace DataTableTests {

procdata data_for_test
    {1251, 1, 500, true, "test_path"};
procdata data_for_test1
    {1252, 1, 500, true, "test_path"};
procdata data_for_test2
    {1253, 1, 500, true, "test_path"};
procdata data_for_test3
    {1254, 1, 500, true, "test_path"};

TEST(DataTable, ApplyData_Equal) {
    DataTable table_for_test;
    table_for_test.applyData(data_for_test );
    table_for_test.applyData(data_for_test1);
    table_for_test.applyData(data_for_test2);
    table_for_test.applyData(data_for_test3);
    auto map = table_for_test.get();
    auto item = map.operator[](1252);
    EXPECT_EQ(item.pid, 1252);
}

TEST(DataTable, ApplyData_AddRemove) {
    DataTable table_for_test;
    table_for_test.applyData(data_for_test);
    EXPECT_EQ(table_for_test.get().size(), 1);
    data_for_test.isOpen = false;
    table_for_test.applyData(data_for_test);
    EXPECT_EQ(table_for_test.get().size(), 0);
}

}

namespace PacketTest {
TEST(Packet, WriteAndReadProcdata) {
    procdata data_for_test
        {1251, 1, 500, true, "test_path"};
    Packet packet;
    packet.Write(data_for_test);
    packet.WriteLength();

    procdata result;
    char * buff = const_cast<char*>(packet.GetBuffer());
    Packet respacket(buff);
    result = respacket.ReadProcdata();
    EXPECT_EQ(data_for_test.pid, result.pid);
    EXPECT_EQ(data_for_test.ppid, result.ppid);
    EXPECT_EQ(data_for_test.time, result.time);
    EXPECT_EQ(data_for_test.isOpen, result.isOpen);
    EXPECT_EQ(data_for_test.path, result.path);
}
}

