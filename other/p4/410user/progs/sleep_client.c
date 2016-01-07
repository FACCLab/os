/* @file sleep_server.c
 * @brief the sleep server. Receives requests to be woken up after a certain
 * number of ticks.  If a request cannot be completed, sends a failure message 
 *
 * @author Mario Dehesa-Azuara (mdehesaa)
 */

#include <stdio.h>
#include <syscall.h>
#include <ipc_client.h>

#include <udriv_registry.h>

#include <simics.h>

#include "sleep_server.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("usage: %s NUMBER\n", argv[0]);
        return 0;
    }

    int ret;
    sleep_req_t req;
    req.cmd = CMD_SLEEP;
    req.data = atoi(argv[1]);

    lprintf("Sleep Client: GOING TO SLEEP AT %u", get_ticks());

    if (ipc_client_send_msg(UDR_SLEEP_SERVER, (void *)&req, sizeof(req), 
                            &ret, sizeof(ret)) < 0) {
        printf("Failed to sleep!\n");
        return 1;
    }

    lprintf("Sleep Client: WAKING UP  AT %u with %d", get_ticks(), ret);

    return 0;
}
