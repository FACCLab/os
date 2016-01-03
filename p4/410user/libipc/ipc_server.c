/** @file 410user/libipc/ipc_server.c
 *  @brief IPC server code
 *  @author tchitten F'15
 */
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <syscall.h>
#include <simics.h>
#include "ipc_int.h"
#include "ipc_server.h"

typedef struct senders_node {
    struct senders_node* next;
    driv_id_t sender;
    unsigned short len;
    unsigned short idx;
    unsigned char message[];
} senders_node_t;   

typedef struct ipc_state {
    driv_id_t server;
    senders_node_t*  senders;
    int tid;
    int clients_ready;  // how many clients have completed their message outside of receive
    int pending_wakeup; // whether we've been woken up outside of receive
    size_t buf_len;     // The minimum buffer size receive may be called with
} ipc_server_t;

static void handle_client_message(ipc_state_t* state, request_msg_t* req, bool accept);

int ipc_server_init(ipc_state_t** pstate, driv_id_t req_server) {
    ipc_state_t* state = malloc(sizeof(ipc_state_t));
    if (!state) {
        return -1;
    }
    int res = udriv_register(req_server, 0, 8);
    if (res < 0) {
        free(state);
        return -2;
    }
    state->server = res;
    state->tid = gettid();
    state->senders = NULL;
    state->clients_ready = 0;
    state->pending_wakeup = 0;
    state->buf_len = 0;
    *pstate = state;
    return 0;
}

void ipc_server_deregister(ipc_state_t* state) {
    ASSERT(state != NULL);
    udriv_deregister(state->server);
    ipc_server_cancel(state);
    free(state);
}

int ipc_server_send_i32(ipc_state_t* state, driv_id_t dest, int32_t msg) {
    ASSERT(state != NULL);
    return ipc_server_send_msg(state, dest, &msg, sizeof(int32_t));
}

int ipc_server_send_str(ipc_state_t* state, driv_id_t dest, char* str) {
    ASSERT(state != NULL);
    return ipc_server_send_msg(state, dest, str, strlen(str));
}

int ipc_server_send_msg(ipc_state_t* state, driv_id_t dest, void* _msg, size_t len) {
    ASSERT(state != NULL);
    ASSERT(FITS(len, unsigned short));
    char* msg = _msg;
    request_msg_t req;
    response_msg_t resp;
    req.sender = state->server;
    req.cmd = COMMAND_BYTE;
    req.len = len;
    if (len == 0) {
        len = 1;
    } 
    
    // Send message byte by byte.
    size_t i;
    for (i = 0; i < len; i++) {
        if (req.len) {
            req.byte = msg[i];
        }
        if (udriv_send(dest, req.raw, sizeof(request_msg_t)) < 0) {
            return -1;
        }   
    }

    // Wait for a message from the client saying they received the message.
    while (true) {
        driv_id_t server;
        if (udriv_wait(&server, &resp.raw, NULL) < 0) {
            return -1;
        }
        if (resp.sender == dest) {
            if (resp.status == STATUS_OK) {
                return 0;
            } else {
                return -1;
            }
        } else {
            // Got a message from a different client, don't drop it!
            handle_client_message(state, (request_msg_t*)&resp, true);
        }
    }
}

static senders_node_t* sender_find(senders_node_t* head, driv_id_t sender) {
    if (head == NULL || head->sender == sender) {
        return head;
    } else {
        return sender_find(head->next, sender);
    }
}
static senders_node_t* sender_find_full(senders_node_t* head) {
    if (head == NULL || head->len == head->idx) {
        return head;
    } else {
        return sender_find_full(head->next);
    }
}
static void sender_remove(senders_node_t** phead, driv_id_t sender) {
    senders_node_t** prev = phead;
    senders_node_t*  cur = *phead;
    while (cur != NULL) {
        if (cur->sender == sender) {
            *prev = cur->next;
            return;
        } else {
            prev = &cur->next;
            cur = cur->next;
        }
    }
}

static void handle_client_message(ipc_state_t* state, request_msg_t* req, bool accept) {
    response_msg_t resp;
    resp.sender = state->server;

    // Handle commands.
    if (req->cmd == COMMAND_PING) {
        return;
    }
    if (req->cmd == COMMAND_WAKEUP) {
        state->pending_wakeup = 1;
        return;
    }
    ASSERT(req->cmd == COMMAND_BYTE);

    // First check if this message is too long and return an error if so.
    if (req->len > state->buf_len) {
        resp.status = STATUS_TOOBIG;
        udriv_send(req->sender, resp.raw, sizeof(response_msg_t));
        return;
    }

    // Add this byte to the sender's node, possibly allocating a new node if
    // we've never seen this sender before.
    senders_node_t* node = sender_find(state->senders, req->sender);
    if (node == NULL) {
        if (!accept) {
            // Tell the sender if we're not accepting new messages.
            resp.status = STATUS_NOACCEPT;
            udriv_send(req->sender, resp.raw, sizeof(response_msg_t));
            return;
        } else {
            node = malloc(sizeof(senders_node_t) + req->len);
            if (node == NULL) {
                // Couldn't allocate a node, tell the sender we can't 
                // service their request right now.
                resp.status = STATUS_NOMEM;
                udriv_send(req->sender, resp.raw, sizeof(response_msg_t));
                return;
            } else {
                node->sender = req->sender;
                node->len = req->len;
                node->idx = 0;
                node->next = state->senders;
                state->senders = node;
                resp.status = STATUS_OK;
                udriv_send(req->sender, resp.raw, sizeof(response_msg_t));
            }
        }
    }
    node->message[node->idx] = req->byte;
    node->idx++;
    if (node->idx == node->len) {
        state->clients_ready++;
    }
}

int ipc_server_recv(ipc_state_t* state, driv_id_t* src, void* buffer, size_t max, bool accept) {
    ASSERT(state != NULL);
    ASSERT(state->tid == gettid());
    senders_node_t* finished = NULL;

    if (state->buf_len < max) {
        // Grow our minimum buffer length if the caller increased it.
        state->buf_len = max;
    } else if (state->buf_len == max) {
        // Buffer length stayed the same. Ok.
    } else {
        // Caller shrank their buffer. This is illegal as we may not be able to
        // service client's we've already responded to.
        return -1;
    }

    do {
        // Wake up if we received a wakeup request. This may have happened in 
        // ipc_server_send so we should do it before we wait.
        if (state->pending_wakeup) {
            state->pending_wakeup = 0;
            break;
        }

        // Check if any clients finished their messages. This may have happened
        // in ipc_server_send so we should do it before we wait.
        if (state->clients_ready > 0) {
            state->clients_ready--;
            finished = sender_find_full(state->senders);
            break;
        }

        // Wait for a new message and handle it appropriately.
        driv_id_t driver;
        request_msg_t req;
        if (udriv_wait(&driver, &req.raw, NULL) < 0) {
            return -2;
        }
        handle_client_message(state, &req, accept);

    // Loop if we're accepting clients or until the list of current clients is
    // empty.
    } while (accept || state->senders != NULL);

    if (finished == NULL) {
        // Got a wake up or no more clients to process.
        *src = UDR_NOSERVER;
        return 0;
    } else {
        // A client finished their message.
        ASSERT(finished->idx == finished->len);
        if (src) *src = finished->sender;
        size_t recv_len = finished->len;
        memcpy(buffer, &finished->message, recv_len);
        sender_remove(&state->senders, finished->sender);
        free(finished);
        return recv_len;
    }
}


void ipc_server_cancel(ipc_state_t* state) {
    ASSERT(state != NULL);
    response_msg_t resp;
    resp.status = STATUS_CANCELLED;
    senders_node_t* next, *node = state->senders;
    do {
        udriv_send(node->sender, resp.raw, sizeof(response_msg_t));
        next = node->next;
        free(node);
    } while ((node = next));
    state->senders = NULL;
}

