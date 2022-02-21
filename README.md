# MacOS_ProcMon_winter_2022
Project for MacOS Procmon on 2022 cpp cources

This program can help you to monitoring system events live start and stop program.
Last version: 1.2.0
Authors: Maxim Sheiko, Oleksandr Ilchuk

1) How to install this program
just start pkg file for install

1.1) Manual installation:
- move Procmon2022.app into Application folder
- move Procmon2022Daemon into /Library/Application Support/Procmon2022
- move com.apriorit.procmon2022.daemon.plist to /Library/LaunchDaemons/
- load plist into launchd:
      sudo chown root:wheel /Library/LaunchDaemons/com.apriorit.procmon2022.daemon.plist
      sudo launchctl load /Library/LaunchDaemons/com.apriorit.procmon2022.daemon.plist

2) How to uninstall this program?
just start uninstall
2.1) Manual uninstaliation:
- sudo rm -rf /Applications/Procmon2022.app
            /Library/Application\ Support/Procmon2022 
            /Library/LaunchDaemons/com.apriorit.procmon2022.daemon.plist

