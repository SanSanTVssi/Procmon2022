///
///  main.swift
///  UNITTEST
///
///  Created by Ааи on 18.02.2022.
///

import Foundation

func Expect(_ expectation: Bool) {
    if (expectation) {
        print("passed")
    }
    else {
        print("failed")
    }
}

UIClientWrapper.ApplyData(pid: 10, ppid: 1, time: 1, isOpen: true, path: "path")
UIClientWrapper.ApplyData(pid: 20, ppid: 1, time: 1, isOpen: true, path: "path")
UIClientWrapper.ApplyData(pid: 33, ppid: 1, time: 1, isOpen: true, path: "path")
UIClientWrapper.ApplyData(pid: 44, ppid: 1, time: 1, isOpen: true, path: "path")
UIClientWrapper.ApplyData(pid: 58, ppid: 1, time: 1, isOpen: true, path: "path")

func UIClientWrapperGetBeginTest() {
    print("UIClientWrapper.Get_Begin_Test")
    let beg = UIClientWrapper.begin()
    Expect(beg.pid == 10)
    print("===============================")
}

func UIClientWrapperGetNextTest() {
    print("UIClientWrapper.Get_Next_Test")
    let next = UIClientWrapper.next()
    Expect(next.pid == 20)
    print("===============================")
}

func UIClientWrapperGetPrevTest() {
    print("UIClientWrapper.Get_Prev_Test")
    let prev = UIClientWrapper.prev()
    Expect(prev.pid == 10)
    print("===============================")
}

func UIClientWrapperGetEndTest() {
    print("UIClientWrapper.Get_End_Test")
    let end = UIClientWrapper.end()
    Expect(end.pid == 0)
    print("===============================")
}

func UIClientWrapperGetNextEndTest() {
    print("UIClientWrapper.Get_Next_End_Test")
    UIClientWrapper.end()
    let nexed = UIClientWrapper.next()
    Expect(nexed.pid == 0)
    print("===============================")
}

func UIClientWrapperGetPrevBeginTest() {
    print("UIClientWrapper.Get_Prev_End_Test")
    UIClientWrapper.begin()
    let pbeg = UIClientWrapper.prev()
    Expect(pbeg.pid == 0)
    print("===============================")
}

func UIClientWrapperSizeTest() {
    print("UIClientWrapper.Size_Test")
    let size = UIClientWrapper.size()
    Expect(size == 5 + 1)
    print("===============================")
}

func UIClientWrapperIsBeginTest() {
    print("UIClientWrapper.isBegin_Test")
    UIClientWrapper.begin()
    let isBeg = UIClientWrapper.isBegin()
    Expect(isBeg)
    print("===============================")
}

func UIClientWrapperIsEndTest() {
    print("UIClientWrapper.isEnd_Test")
    UIClientWrapper.end()
    let isEnd = UIClientWrapper.isEnd()
    Expect(isEnd)
    print("===============================")
}

func UIClientWrapperGetTable() {
    print("UIClientWrapper.GetTable_Test")
    let table = UIClientWrapper.GetTable()
    Expect(table[0].pid == 10)
    Expect(table[1].pid == 20)
    Expect(table[2].pid == 33)
    Expect(table[3].pid == 44)
    Expect(table[4].pid == 58)
    print("===============================")
}

UIClientWrapperGetBeginTest()
UIClientWrapperGetNextTest()
UIClientWrapperGetPrevTest()
UIClientWrapperGetEndTest()
UIClientWrapperGetNextEndTest()
UIClientWrapperGetPrevBeginTest()
UIClientWrapperSizeTest()
UIClientWrapperIsBeginTest()
UIClientWrapperIsEndTest()
UIClientWrapperGetTable()
