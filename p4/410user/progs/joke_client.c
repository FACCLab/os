/* @author Funny you should ask... */

#include <stdio.h>
#include <stdbool.h>
#include <simics.h>
#include <syscall.h>
#include <ipc_client.h>

#define MAX_JOKE_LEN 1024
char joke[MAX_JOKE_LEN];

int main() {

    uint8_t cmd = 1;
    if (ipc_client_send_i32(UDR_JOKE_SERVER, cmd, &joke, MAX_JOKE_LEN) < 0) {
        printf("could not retrieve a joke, exiting...\n");
        return -1;
    }
    printf("%s\n", joke);

    return 0;
}
