/** @file 410user/libipc/ipc_client.c
 *  @brief IPC client code
 *  @author tchitten F'15
 */
#include <assert.h>
#include <string.h>
#include <simics.h>
#include <udriv_registry.h>
#include "ipc_client.h"
#include "ipc_int.h"

static int send_msg(driv_id_t src, driv_id_t dst, char* msg, size_t len) {
    request_msg_t req;
    response_msg_t resp;
    req.sender = src;
    req.cmd = COMMAND_BYTE;
    req.len = len;
    if (len == 0) {
        len++;
    }

    size_t i;
    for (i = 0; i < len; i++) {
        if (req.len) {
            req.byte = msg[i];
        }
        if (udriv_send(dst, req.raw, sizeof(request_msg_t)) < 0) {
            goto fail;
        }
    }

    // Wait for a response saying the server will receive the message.
    driv_id_t server;
    if (udriv_wait(&server, &resp.raw, NULL) < 0) {
        goto fail;
    }
    if (server != src) {
        // Received a byte for the wrong server. This likely corrupted someon else.
        panic("client_send: tid %d received message for wrong server %d", gettid(), server);
    }
    if (resp.sender != dst) {
        // Received a byte from the wrong sender. This likely corrupted someone else.
        panic("client_send: tid %d received message from wrong sender %d", gettid(), resp.sender);
    }
    if (resp.status != STATUS_OK) {
        goto fail;
    }   

    return 0;
fail:
    return -1;
}

// Dst is us, src is them.
static int recv_msg(driv_id_t dst, driv_id_t src, char* buf, size_t max, size_t* _len) {
    request_msg_t req;
    response_msg_t resp;
    resp.sender = dst;
    size_t i = 0, len = max;
    int acked = 0;
    while (i < len) {
        // Receive a message, hopefully only for the server we expect!
        driv_id_t driver;
        if (udriv_wait(&driver, &req.raw, NULL) < 0) {
            return -1;
        }
        if (driver != dst) {
            // Received a byte for the wrong sender.
            panic("client_recv: tid %d received message for wrong server %d", gettid(), driver);
        }

        // Acknowledge the sender saying whether we can accept the message or not.
        if (req.len > max) {
            resp.status = STATUS_TOOBIG;
            udriv_send(src, resp.raw, sizeof(response_msg_t));
            return -1;
        } else if (!acked) { 
            acked = 1;
            len = req.len;
            resp.status = STATUS_OK;
            udriv_send(src, resp.raw, sizeof(response_msg_t));
        }

        // Handle various commands.
        if (req.cmd == COMMAND_PING) {
            continue;
        }
        if (req.cmd == COMMAND_WAKEUP) {
            // Can't wake up a client.
            panic("client_recv: can't wakeup a client");
        }
        if (req.cmd == COMMAND_CANCEL) {
            return -1;
        }
        
        // Receive the byte.
        ASSERT(req.cmd == COMMAND_BYTE);
        buf[i++] = req.byte;
    }
    *_len = len;

    return 0;
}
    
int ipc_client_send_msg(driv_id_t dst, void* msg, size_t len, void* resp, size_t max_resp_len) {
    ASSERT(FITS(len, unsigned short));
    int server = udriv_register(UDR_ASSIGN_REQUEST, 0, sizeof(response_msg_t));
    if (server < 0) {
        return -1;
    }
   
    if (send_msg(server, dst, msg, len) < 0) {
        udriv_deregister(server);
        return -1;
    }

    size_t resp_len;
    if (max_resp_len > 0 && recv_msg(server, dst, resp, max_resp_len, &resp_len) < 0) {
        udriv_deregister(server);
        return -1;
    }

    udriv_deregister(server);
    return resp_len;
}

int ipc_client_send_str(driv_id_t dst, char* str, void* resp, size_t resp_len) {
    return ipc_client_send_msg(dst, str, strlen(str), resp, resp_len);
}

int ipc_client_send_i32(driv_id_t dst, int32_t msg, void* resp, size_t resp_len) {
    return ipc_client_send_msg(dst, &msg, sizeof(msg), resp, resp_len);
}

int ipc_wakeup(driv_id_t dest) {
    request_msg_t req;
    req.sender = UDR_NOSERVER;
    req.cmd = COMMAND_WAKEUP;
    return udriv_send(dest, req.raw, sizeof(request_msg_t));
}

int ipc_ping(driv_id_t dest) {
    request_msg_t req;
    req.sender = UDR_NOSERVER;
    req.cmd = COMMAND_PING;
    return udriv_send(dest, req.raw, sizeof(request_msg_t));
}
