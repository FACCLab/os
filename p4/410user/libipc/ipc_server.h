/** @file 410user/libipc/ipc_server.h
 *  @brief IPC server spec
 *  @author tchitten F'15
 */
#ifndef _IPC_SERVER_H_
#define _IPC_SERVER_H_

#include <syscall.h>
#include <stdbool.h>
#include <udriv_registry.h>

/* a per-thread opaque structure of server state */
typedef struct ipc_state ipc_state_t;

/* @brief initializes a thread to receive messages for a given server id
 *
 * @param pstate: reference to an ipc_state pointer that init will populate 
 *                the with a newly allocated state object.
 * @param server: the requested server id
 * 
 * @return error if the server is already registered, otherwise the id that the
 * thread registerd for.
 */
int ipc_server_init(ipc_state_t **pstate, driv_id_t server);

/* @brief destroys and frees an active IPC state. All memory is freed, all 
 * senders with partial requests will be notified of failure, and the server ID
 * will be deregistered.
 *
 * @param state: the ipc state that is being de-initialized
 * @return void
 */
void ipc_server_deregister(ipc_state_t* state);

/* @brief sends an arbitrary-length message to a destination server
 * 
 * @param state: the calling thread's initialized IPC state 
 * @param dest: the destination of the message
 * @param msg: a pointer to the message being sent
 * @param len: the size of the message being sent
 *
 * @return error if sender refuses transfer or if is otherwise unable to accept
 * the mssage.  0 otherwise.
 */
int ipc_server_send_msg(ipc_state_t*, driv_id_t dest, void* msg, size_t len);

/* @brief sends a null-terminated string message to a destination server 
 * 
 * @param state: the calling thread's initialized IPC state 
 * @param dest: the destination of the message
 * @param str: the string message being sent
 * @param len: the length of the string message 
 *
 * @return error if sender refuses transfer or if is otherwise unable to accept
 * the mssage.  0 otherwise.
 */
int ipc_server_send_str(ipc_state_t*, driv_id_t dest, char* str);

/* @brief sends an integer message to a destination server 
 * 
 * @param state: the calling thread's initialized IPC state 
 * @param dest: the destination of the message
 * @param msg: the integer message being sent 
 *
 * @return error if sender refuses transfer or if is otherwise unable to accept
 * the mssage.  0 otherwise.
 */
int ipc_server_send_i32(ipc_state_t*, driv_id_t dest, int32_t msg);

/* @brief synchronously receives a message from a client
 *
 * @param state: the calling thread's initialized IPC state
 * @param src: a pointer to a driv_id_t that will be populated with the sender's 
 *             id. If the server was woken up due to a call to ipc_wakeup, this
 *             field will be populated with the UDR_NOSERVER constant.
 * @param buffer: a buffer for the sender's message
 * @param max: the size of the buffer
 * @param accept: whether new clients should be accepted or only clients
 *                currently sending this is useful for "winding down" a server 
 *                gracefully and not cancelling any pending requests
 * 
 * @return error if a message could not be received. the size of the received
 * message otherwise.
 */
int ipc_server_recv(ipc_state_t *state, driv_id_t* src, void* buffer, size_t max, bool accept);

/* @brief cancels any pending IPC requests */
void ipc_server_cancel(ipc_state_t *state);

#endif
