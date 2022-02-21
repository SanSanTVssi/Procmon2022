///
///  ViewController.swift
///  Procmon
///
///  Created by Student on 09.02.2022.
///

import Cocoa

func StringQToString(_ str_q: String?) -> String {
    var str: String
    let str_q = str_q
    if (str_q == nil) {
        str = ""
    }
    else {
        str = str_q!
    }
    return str;
}


class ViewController: NSViewController, NSTableViewDelegate, NSTableViewDataSource {
    dynamic var procdata : [SwiftProcdata] = []
    
    @IBOutlet var tableView : NSTableView!
    @IBOutlet var searchField : NSTextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        UIClientWrapper.pipeMonitorInit()
        procdata = UIClientWrapper.GetTable()
        tableView?.dataSource = self
        tableView?.delegate = self
        tableView?.usesStaticContents = false
        _ = Task.detached {
            self.update()
        }
        // Do any additional setup after loading the view.
    }
    
    override var representedObject: Any? {
        didSet {
            // Update the view, if already loaded.
        }
    }
    
    func update()
    {
        while true
        {
            DispatchQueue.main.async {
                self.procdata = UIClientWrapper.GetTable(searchString: self.searchField.stringValue);
                self.tableView?.reloadData();
            }
            sleep(1)
        }
    }
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        return procdata.count
    }
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        let procdataInst = procdata[row]
        guard let cell = tableView.makeView(withIdentifier: tableColumn!.identifier, owner: self) as? NSTableCellView else {return nil}
        
        if((tableColumn?.identifier)!.rawValue == "pid")
        {
            cell.textField?.stringValue = String(procdataInst.pid)
        } else if(tableColumn?.identifier)!.rawValue == "ppid"
        {
            cell.textField?.stringValue = String(procdataInst.ppid)
        } else if((tableColumn?.identifier)!.rawValue == "time")
        {
            cell.textField?.stringValue = StringQToString(procdataInst.time_str)
        } else if((tableColumn?.identifier)!.rawValue == "path")
        {
            cell.textField?.stringValue = StringQToString(procdataInst.path)
        } else if((tableColumn?.identifier)!.rawValue == "sha1_hash")
        {
            cell.textField?.stringValue = StringQToString(procdataInst.sha1Hash)
        } else if((tableColumn?.identifier)!.rawValue == "name")
        {
            let runningApp = NSRunningApplication(processIdentifier: pid_t(procdataInst.pid))
            cell.imageView?.image = runningApp?.icon
            cell.textField?.stringValue = StringQToString(procdataInst.name)
        }
        return cell
    }
}
