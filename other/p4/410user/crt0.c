/* @file crt0.c
 * @brief The 15-410 userland C runtime setup function
 *
 * @note The build infrastructure "knows" that _main is the entrypoint
 *       function and will do the right thing as a result.
 */

#include <stdlib.h>
#include <syscall.h>
#include <assert.h>
#include <stdint.h>
#include <simics.h>
#include <string.h>

#define READLINE_MAGIC 0x15410f00
#define CONSOLE_MAGIC 0x15410bad

typedef union {
    message_t raw; 
    struct {
        uint32_t magic;
        uint32_t value;
    };
} exec_msg_t;

void post_exec();
extern int main(int argc, char *argv[]);
extern void install_autostack(void * stack_high, void * stack_low);
extern void readline_set_server(driv_id_t server);
extern void console_set_server(driv_id_t server);
extern driv_id_t readline_get_server();
extern driv_id_t console_get_server();

void _main(int argc, char *argv[], void *stack_high, void *stack_low)
{
    post_exec();
    install_autostack(stack_high, stack_low);
    exit(main(argc, argv));
}

// Leave ourselves a note about what our readline/print servers should be.
int pre_exec() {
    int mailbox;
    if ((mailbox = udriv_register(UDR_ASSIGN_REQUEST, 0, 8)) < 0) {
        goto fail;
    }
    
    exec_msg_t msg;
    msg.magic = READLINE_MAGIC;
    msg.value = readline_get_server();
    if (udriv_send(mailbox, msg.raw, sizeof(exec_msg_t)) < 0) {
        goto fail;
    }

    msg.magic = CONSOLE_MAGIC;
    msg.value = console_get_server();
    if (udriv_send(mailbox, msg.raw, sizeof(exec_msg_t)) < 0) {
        goto fail;
    }

    return 0;
fail:
    return -1;
}

// Retrieve the note about what our readline/print servers are.
void post_exec() {
    exec_msg_t msg;
    driv_id_t mb;
    unsigned int len;
    int recvd = 0;
    while (recvd < 2) {
        if (udriv_wait(&mb, &msg.raw, &len) < 0) {
            // Not registered for a server so we're probably the first process.
            break;
        }
        switch(msg.magic) {
            case READLINE_MAGIC: 
                readline_set_server(msg.value); 
                break;
            case CONSOLE_MAGIC: 
                console_set_server(msg.value); 
                break;
            default: continue;
        }
        recvd++;
    }
}
