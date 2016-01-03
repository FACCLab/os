/** @file 410user/libipc/ipc_client.h
 *  @brief IPC client spec
 *  @author tchitten F'15
 */
#ifndef _IPC_CLIENT_H_
#define _IPC_CLIENT_H_

#include <syscall.h>
#include <stdbool.h>
#include <udriv_registry.h>

/* @brief send an arbitrary-length message to a destination server
 *
 * @param dest: the destination of the message
 * @param len: the length of the message
 * @param resp: a pointer to a buffer the response is to be put in, or NULL if 
 *              no response is expected
 * @param resp_len: the size of the response buffer or 0 if no response is 
 *                  expected
 *
 * @return error if no sender refuses transfer or if is otherwise unable to
 * accept this message. 0 otherwise.
 */
int ipc_client_send_msg(driv_id_t dest, void* msg, size_t len, 
                        void* resp, size_t resp_len);

/* @brief sends a null terminated string to a destination server
 *
 * @param dest: the destination of the message
 * @param str: the null terminated string to send the user
 * @param resp: a pointer to a buffer ther response is to be put in, or NULL if
 *              no response is expected
 * @param resp_len: the size of the response buffer or 0 if no response is 
 *                  expected
 *
 * @return error if no sender refuses transfer or if is otherwise unable to
 * accept this message. 0 otherwise.
 */
int ipc_client_send_str(driv_id_t dest, char* str, 
                        void* resp, size_t resp_len);

/* @brief sends a 4-byte integer to a destination server
 *
 * @param dest: the destination of the message
 * @param msg: the integer to send
 * @param resp: a pointer to a buffer ther response is to be put in, or NULL if
 *              no response is expected
 * @param resp_len: the size of the response buffer or 0 if no response is 
 *                  expected
 *
 * @return error if no sender refuses transfer or if is otherwise unable to
 * accept this message. 0 otherwise.
 */
int ipc_client_send_i32(driv_id_t dest, int32_t msg, 
                        void* resp, size_t resp_len);

/* @brief wakes up a receiving server so they can process any pending work
 *
 * This is useful in driver programs where a driver may want to notify a server
 * that it has new bytes to send any clients.
 *
 * @param dest: the IPC server to wake up.
 * @return error if no such server exists. 0 otherwise.
 */
int ipc_wakeup(driv_id_t dest);

/* @brief checks if a server is alive
 *
 * @param dest: the IPC server to ping
 * @return error if no such server exists. 0 otherwise.
 */
int ipc_ping(driv_id_t dest);

#endif
