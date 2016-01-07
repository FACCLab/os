/** @file 410user/libipc/ipc_int.h
 *  @brief IPC library internal definitions
 *  @author tchitten F'15
 */
#ifndef _IPC_INT_H_
#define _IPC_INT_H_

#define ASSERT(x) assert(x)
#define FITS(x, t) (x == (typeof(x))(t)x)

#define COMMAND_BYTE 0
#define COMMAND_CANCEL 1
#define COMMAND_PING 2
#define COMMAND_WAKEUP 3
typedef union {
    message_t raw;
    struct {
        unsigned int sender;
        unsigned short len;
        unsigned char cmd;
        unsigned char byte;
    };
} request_msg_t;

#define STATUS_OK 0
#define STATUS_DONE 1
#define STATUS_NOMEM 2
#define STATUS_TOOBIG 3
#define STATUS_NOACCEPT 4
#define STATUS_CANCELLED 5
typedef union {
    message_t raw;
    struct {
		unsigned int sender;
        unsigned int status;
    };
} response_msg_t;

#endif
