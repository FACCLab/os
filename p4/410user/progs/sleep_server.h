/* @file sleep_server.h
 * @brief the sleep server. Receives requests to be woken up after a certain
 * number of ticks.  If a request cannot be completed, sends a failure message 
 *
 * @author Amanda M. Watson (amwatson)
 */

#ifndef _SLEEP_SERVER_H_
#define _SLEEP_SERVER_H_

#define CMD_SLEEP 1
#define CMD_INTERNAL 2

typedef struct sleep_req {
    int cmd;
    unsigned long long data;
} sleep_req_t;

#define SLEEP_SUCCESS 42
#define SLEEP_FAILURE 43

#endif /* _SLEEP_SERVER_H_ */
