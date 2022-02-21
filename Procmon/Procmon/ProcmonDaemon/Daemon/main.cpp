///
/// Program entry point
/// In this file the main process forked
/// 
/// Created by AAI2002 on 20.01.2022
/// 

#include "Daemon.hpp"

int main() {
    /// Initialize daemon
    ProcmonDaemon::init();
    return 0;
}
