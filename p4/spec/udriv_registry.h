/* @file udriv_registry.h
 * @brief definitions for servers, devices and their drivers
 *
 * @author Amanda M. Watson
 */

#ifndef UDRIV_REGISTRY_H
#define UDRIV_REGISTRY_H

typedef enum 
{
    // A driver value indicating that no server was specified.
    UDR_NOSERVER,

    // Define devices here 
    UDR_KEYBOARD,
    UDR_MOUSE,
    UDR_CONSOLE,
    UDR_DEV_COM1, 
    UDR_DEV_COM2,
    UDR_DEV_COM3,
    UDR_DEV_COM4,

    // Anything above this point can be a userland server ID. 
    UDR_MAX_HW_DEV = 16,

    // Identities of well-known servers
    UDR_READLINE_SERVER,
    UDR_MOUSE_SERVER,
    UDR_SLEEP_SERVER,
    UDR_NAME_SERVER,
    UDR_KEYBOARD_SERVER,
    UDR_JOKE_SERVER,
    UDR_COM1_READLINE_SERVER,
    UDR_COM2_READLINE_SERVER,
    UDR_COM3_READLINE_SERVER,
    UDR_COM4_READLINE_SERVER,
    UDR_COM1_PRINT_SERVER,
    UDR_COM2_PRINT_SERVER,
    UDR_COM3_PRINT_SERVER,
    UDR_COM4_PRINT_SERVER,

    // Not a valid ID:
    // Pass to udriv_register() to request kernel to assign a mailbox i.d.
    UDR_ASSIGN_REQUEST = 999,

    // All i.d.'s above this point are kernel-assigned
    UDR_MIN_ASSIGNMENT
} driv_id_t;

#endif /* UDRIV_REGISTRY_H */
