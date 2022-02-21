///
///  PreMonitor.cpp
///  Daemon
///
///  Created by Ааи on 19.02.2022.
///

#include "Monitor.hpp"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sysctl.h>
#include <errno.h>
#include <libproc.h>

namespace PreMonitor {
std::string PathByPid(int pid) {
    unsigned int const buffsize = 1024;
    char buffer[buffsize];
    auto ret = proc_pidpath(pid, buffer, buffsize);
    if (ret != -1) {
        return std::string(buffer);
    }
    else {
        return {};
    }
}

std::string NameByPid(int pid) {
    unsigned int const buffsize = 1024;
    char buffer[buffsize];
    auto ret = proc_name(pid, buffer, buffsize);
    if (ret != -1) {
        return std::string(buffer);
    }
    else {
        return {};
    }
}

int GetBSDProcessList(kinfo_proc **procList, size_t *procCount)
    /// Returns a list of all BSD processes on the system.  This routine
    /// allocates the list and puts it in *procList and a count of the
    /// number of entries in *procCount.  You are responsible for freeing
    /// this list (use "free" from System framework).
    /// On success, the function returns 0.
    /// On error, the function returns a BSD errno value.
{
    int err;
    kinfo_proc * result;
    bool done;
    static const int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    /// Declaring name as const requires us to cast it when passing it to
    /// sysctl because the prototype doesn't include the const modifier.
    size_t              length;

    assert( procList != NULL);
    assert(*procList == NULL);
    assert(procCount != NULL);

    *procCount = 0;

    /// We start by calling sysctl with result == NULL and length == 0.
    /// That will succeed, and set length to the appropriate length.
    /// We then allocate a buffer of that size and call sysctl again
    /// with that buffer.  If that succeeds, we're done.  If that fails
    /// with ENOMEM, we have to throw away our buffer and loop.  Note
    /// that the loop causes use to call sysctl with NULL again; this
    /// is necessary because the ENOMEM failure case sets length to
    /// the amount of data returned, not the amount of data that
    /// could have been returned.

    result = NULL;
    done = false;
    do {
        assert(result == NULL);

        /// Call sysctl with a NULL buffer.

        length = 0;
        err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                      NULL, &length,
                      NULL, 0);
        if (err == -1) {
            err = errno;
        }

        /// Allocate an appropriately sized buffer based on the results
        /// from the previous call.

        if (err == 0) {
            result = (kinfo_proc * ) malloc(length);
            if (result == NULL) {
                err = ENOMEM;
            }
        }

        /// Call sysctl again with the new buffer.  If we get an ENOMEM
        /// error, toss away our buffer and start again.

        if (err == 0) {
            err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                          result, &length,
                          NULL, 0);
            if (err == -1) {
                err = errno;
            }
            if (err == 0) {
                done = true;
            } else if (err == ENOMEM) {
                assert(result != NULL);
                free(result);
                result = NULL;
                err = 0;
            }
        }
    } while (err == 0 && ! done);

    /// Clean up and establish post conditions.

    if (err != 0 && result != NULL) {
        free(result);
        result = NULL;
    }
    *procList = result;
    if (err == 0) {
        *procCount = length / sizeof(kinfo_proc);
    }

    assert( (err == 0) == (*procList != NULL) );

    return err;
}

procdata Convert(kinfo_proc data) {
    procdata to_return;
    to_return.pid = data.kp_proc.p_pid;
    to_return.ppid = data.kp_eproc.e_ppid;
    auto t = data.kp_proc.p_un.__p_starttime;
    to_return.time = (t.tv_sec * 1000000ULL + t.tv_usec) * 1000;
    to_return.isOpen = true;
    to_return.path = PathByPid(to_return.pid);
    to_return.name = NameByPid(to_return.pid);
    return to_return;
}

void SetListOfAlreadyRunningProcesses(DataTable& table) {
    kinfo_proc *procList = NULL;
    size_t procCount;
    if (GetBSDProcessList(&procList, &procCount)) {
        throw std::runtime_error("Error while GetBSDProcessList!");
    }
    else {
        for (int i = 0; i < procCount; i++) {
            table.ApplyData(Convert(procList[i]));
        }
    }
    free(procList);
}

}
