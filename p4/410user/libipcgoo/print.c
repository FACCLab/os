#include <ipc_client.h>

static driv_id_t server = 0;

void console_set_server(driv_id_t new_server) {
    server = new_server;
}

driv_id_t console_get_server() {
    return server;
}

int sys_print(int size, char* buf);
int print(int size, char* buf) {
    if (server == 0) {
        return sys_print(size, buf);
    } else {
        return ipc_client_send_msg(server, buf, size, NULL, 0);
    }
}
