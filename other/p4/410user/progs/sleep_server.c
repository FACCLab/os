/* @file sleep_server.c
 * @brief the sleep server. Receives requests to be woken up after a certain
 * number of ticks.  If a request cannot be completed, sends a failure message 
 *
 * @author Amanda M. Watson (amwatson)
 */

#include <stdio.h>
#include <string.h>

#include <syscall.h>
#include <simics.h>
#include <assert.h>

#include <thread.h>
#include <mutex.h>
#include <sem.h>

#include <ipc_server.h>
#include <ipc_client.h>
#include <udriv_registry.h>

#include "sleep_server.h"

typedef struct sleep_node_t
{
    unsigned int tick;
    driv_id_t wakeup_id;
    struct sleep_node_t *next;

} sleep_node_t;

sleep_node_t *sleep_head;

mutex_t sleep_lock;

void wakeup_threads(int tick_cur)
{
    mutex_lock(&sleep_lock);
    sleep_node_t *node = sleep_head;
    while (node != NULL)
    {
        sleep_node_t *old_node = node; 
        if (node->tick > tick_cur)
        {
            break;
        }

        lprintf("Sleep Server: Helper waking server to wake %u at tick %u",
                node->wakeup_id, node->tick);

        sleep_req_t req;
        req.cmd = CMD_INTERNAL;
        req.data = node->wakeup_id;

        void *nothing = (void *)0xdeadbeef;
        ipc_client_send_msg(UDR_SLEEP_SERVER, (void *)&req, sizeof(req), nothing, 0);

        sleep_head = node = node->next;

        free(old_node);
    }
    mutex_unlock(&sleep_lock);

}

void sorted_insert(sleep_node_t *node_insert)
{
    mutex_lock(&sleep_lock);

    sleep_node_t *node_cur = sleep_head;
    sleep_node_t *prev_node = NULL;
    while (node_cur != NULL)
    {
        if (node_cur->tick > node_insert->tick)
        {
            break;
        }
        prev_node = node_cur;
        node_cur = node_cur->next;
    }

    if (prev_node)
    {
        prev_node->next = node_insert;
    }
    else
    {
        sleep_head = node_insert; 
    }

    node_insert->next = node_cur;

    mutex_unlock(&sleep_lock);
}

void *wakeup_thread_func(void *dummy)
{
    while (1)
    {
        int ticks = get_ticks();
        wakeup_threads(ticks);    
        sleep(1);
    }

}

int sleep_d_init()
{
    if (mutex_init(&sleep_lock) < 0)
    {
        return -2;
    }

    if (thr_create(wakeup_thread_func, NULL) < 0)
    {
        return -4;
    }

    return 0;
}


int main()
{
    int pid;
    if ((pid = fork()) != 0) {
        if (pid < 0) {
            printf("sleep server could not be started\n");
            return -1;
        } else {
            printf("sleep server started on pid %d\n", pid);
            return 0;
        }
    }    

    if (thr_init(4096) < 0)
    {
        return -1;
    }

    ipc_state_t *state;
    if (ipc_server_init(&state, UDR_SLEEP_SERVER) < 0) {
        printf("could not register for sleep server, exiting...\n");
        return -1;
    }

    if (sleep_d_init() < 0)
    {
        lprintf("ERROR: Failed to init sleepd!");
        return -1;
    }


    while (1)
    {
    
        driv_id_t sender = 0;

        lprintf("Sleep Server: Waiting for sleep client...");

        sleep_req_t sleep_req;
        int msg_size = ipc_server_recv(state, &sender, &sleep_req,
                                       sizeof(sleep_req_t), true);


        if (msg_size < 0) {
            lprintf("Sleep Server: Could not receive request, exiting...");
            return -1;
        }

        if (msg_size == sizeof(sleep_req_t)) {
            if (sleep_req.cmd == CMD_SLEEP) {
                lprintf("Sleep Server: Got a sleep request from %u for %llu", 
                         sender, sleep_req.data);
                sleep_node_t *new_node = malloc(sizeof(*new_node));
                if (!new_node)
                {
                    ipc_wakeup(sender);
                    continue;
                } 
                
                new_node->wakeup_id = sender;
                new_node->tick = sleep_req.data + get_ticks();
                new_node->next = NULL;
                sorted_insert(new_node);

            } else {
                lprintf("Sleep Server: about to wake up %llu at tick %u", 
                        sleep_req.data, get_ticks());
                ipc_server_send_i32(state, sleep_req.data, SLEEP_SUCCESS);
            }
        }
    }

    return 0;
}

