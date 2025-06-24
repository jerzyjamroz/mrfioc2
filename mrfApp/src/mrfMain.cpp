/* mrfMain.cpp */
/* Author:  Marty Kraimer Date:    17MAR2000 */

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"

#include <csignal>

// Signal handler function
void handle_sigterm(int signum)
{

    iocshCmd("dbpf VAN:EVR-1:DCEna-Sel Disable");
    iocshCmd("dbpf");
    iocshCmd("date");
    printf("&&&&&&&&& mrfMain.cpp SIGTERM &&&&&&&&&&&\n");
    // Optionally, exit the program
    // exit(0);
}

int main(int argc, char *argv[])
{
    printf("---------------------!!! mrfMain.cpp !!!---------------------\n");
    // Register the signal handler for SIGTERM
    std::signal(SIGTERM, handle_sigterm);
    if (argc >= 2)
    {
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    printf("=================================!!! mrfMain.cpp EXIT !!!=================================\n");
    epicsExit(0);
    return (0);
}
