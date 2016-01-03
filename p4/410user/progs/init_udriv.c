/** @file 410user/progs/init_udriv.c
 *  @author Amanda M. Watson et al.
 *  @brief Initial program.
 *  @public yes
 *  @status ?
 */

#include <syscall.h>
#include <stdio.h>
#include <simics.h>
#include <ipc_client.h>
#include <udriv_registry.h>
#include <assert.h>

#define NSHELLS 3
driv_id_t shell_readlines[NSHELLS] = { 
    UDR_READLINE_SERVER,
    UDR_COM1_READLINE_SERVER,
    UDR_COM2_READLINE_SERVER,
};
driv_id_t shell_prints[NSHELLS] = {
    0, // indicates to the print wrapper to go through sys_print
    UDR_COM1_PRINT_SERVER,
    UDR_COM2_PRINT_SERVER,
};
int shell_tids[NSHELLS] = { 0, 0, 0 };

extern void readline_set_server(driv_id_t serv);
extern void console_set_server(driv_id_t serv);

// Synchronously begins a server
int daemon_create(char *server_prog, char* arg, driv_id_t server_id)
{
    int server_tid = fork();
    if (server_tid < 0) {
        return -1;
    }

    // if child server, begin program execution
    if (!server_tid) {
        char* args1[] = {server_prog, 0};
        char* args2[] = {server_prog, arg, 0};
        char** args = arg == NULL ? args1 : args2; // yuck.
        if (exec(server_prog, args) < 0) {
            // couldn't exec :(
            return -2;
        }
    }

    while (ipc_ping(server_id) < 0) {
        yield(server_tid);
    }
    
    return 0;
}


int daemon_init() {
    // system cannot proceed without these core resources
    assert(daemon_create("readline_server", NULL, UDR_READLINE_SERVER) >= 0);
    assert(daemon_create("serial_server", "COM1", UDR_COM1_READLINE_SERVER) >= 0);
    assert(daemon_create("serial_server", "COM2", UDR_COM2_READLINE_SERVER) >= 0);

    return 0;
}

int shell_create(int idx) {
    int shell_tid = fork();
    if (shell_tid < 0) {
        return -1;
    }   
    
    shell_tids[idx] = shell_tid;
    if (!shell_tid) {
        readline_set_server(shell_readlines[idx]);
        console_set_server(shell_prints[idx]);
        char* args[] = {"shell", 0};
        if (exec("shell", args) < 0) {
            return -2;
        }
    }

    return 0;
}

int main()
{
    if (daemon_init() < 0) {
        panic("init failed to init system daemons!");
    }

    int i;
    for (i = 0; i < NSHELLS; i++) {
        if (shell_create(i) < 0) {
            panic("could not spawn initial shell");
        }
    }

    while(1) {
        int exitstatus, tid = wait(&exitstatus);
        for (i = 0; i < NSHELLS; i++) {
            if (tid == shell_tids[i]) {
                console_set_server(shell_prints[i]);
                printf("Shell exited with status %d, starting it back up...\n", exitstatus);
                if (shell_create(i) < 0) {
                    printf("Could not respawn shell...\n");
                }
            }
        }
    }
}
