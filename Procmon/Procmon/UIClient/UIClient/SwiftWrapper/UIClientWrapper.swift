//
//  Wrapper.swift
//  SwiftParserTest
//
//  Created by Ааи on 04.02.2022.
//

import Foundation

struct SwiftProcdata {
    var pid: Int32
    var ppid: Int32
    var time: UInt64
    var isOpen: Bool
    var path: String!
    var sha1Hash: String!
    var name: String!
    var time_str: String!
}

class UIClientWrapper {
    
    static func ApplyData(pid: Int32,
                          ppid: Int32,
                          time: UInt64,
                          isOpen: Bool,
                          path: String!)
    {
        UIClient.applyData(pid, ppid, time, isOpen, path)
    }
    
    static func pipeMonitorInit() {
        UIClient.pipeMonitorInit()
    }
    
    static func getProcdata() -> SwiftProcdata {
        let pid = UIClient.getPidByIterator()
        
        let ppid = UIClient.getPPid(pid)
        let time = UIClient.getTime(pid)
        let isOpen = UIClient.getIsOpen(pid)
        let path = UIClient.getPath(pid)
        let hash = UIClient.getHash(pid)
        let name = UIClient.getName(pid)
        let time_str = UIClient.getTimeStr(pid)
        
        let to_return = SwiftProcdata.init(pid: pid,
                                           ppid: ppid,
                                           time: time,
                                           isOpen: isOpen,
                                           path: path,
                                           sha1Hash: hash,
                                           name: name,
                                           time_str: time_str)
        
        return to_return
    }
    
    static func isBegin() -> Bool {
        return UIClient.isBegin()
    }
    
    static func isEnd() -> Bool {
        return UIClient.isEnd()
    }

    static func next() -> SwiftProcdata {
        if (!isEnd()) {
            UIClient.increment()
        }
        return getProcdata()
    }

    static func prev() -> SwiftProcdata {
        if (!isBegin()) {
            UIClient.decrement()
        }
        return getProcdata()
    }

    static func begin() -> SwiftProcdata {
        UIClient.begin()
        return getProcdata()
    }

    static func end() -> SwiftProcdata {
        UIClient.end()
        return getProcdata()
    }
    
    static func size() -> UInt {
        return UIClient.size()
    }
    
    static func GetTable()->[SwiftProcdata] {
        var ProcdataArr: [SwiftProcdata] = []
        var value = begin()
        while (!isEnd()) {
            if(value.path != "") {
                ProcdataArr.append(value);
            }
            value = next()
        }
        return ProcdataArr
    }
    static func GetTable(searchString : String) ->[SwiftProcdata]
    {
        if(searchString.isEmpty)
        {
            return self.GetTable()
        }
        else
        {
            var ProcdataArr: [SwiftProcdata] = []
            var value = begin()
            while (!isEnd()) {
                if(value.path.contains(searchString)) {
                    ProcdataArr.append(value);
                    //print(value)
                }
                else if(String(value.pid).contains(searchString)) {
                    ProcdataArr.append(value);
                }
                else if(String(value.ppid).contains(searchString)) {
                    ProcdataArr.append(value);
                }
                else if(String(value.time).contains(searchString)) {
                    ProcdataArr.append(value);
                }
                value = next()
            }
            return ProcdataArr
        }
    }
}
