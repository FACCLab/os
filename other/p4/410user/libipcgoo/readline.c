#include <syscall.h>
#include <simics.h>
#include <ipc_client.h>
#include <udriv_registry.h>

static driv_id_t server = UDR_READLINE_SERVER;

void readline_set_server(driv_id_t new_server) {
    server = new_server;
}

driv_id_t readline_get_server() {
    return server;
}

extern int sys_readline(int, char*);
int readline(int len, char* buf) {
    int res = ipc_client_send_i32(server, len, buf, len);
    if (res < 0) {
        // Maybe udriv syscalls aren't implemented. Fall back to sys_readline.
        res = sys_readline(len, buf);
    }   
    return res;
}
