/* @file device_table.h 
 *
 * @brief Contains the kernel-side lookup table listing devices available for
 * registration by the user.    
 *
 * The contents of this table are specific to this system configuration! 
 * It would be remiss to make implementation decisions that do not support the
 * addition, removal or revision of table entries. 
 *
 * @author Amanda M. Watson (amwatson)
 * @bug No known bugs
 */ 

#include <x86/page.h>

#include <udriv_kern.h>
#include <udriv_registry.h>

const dev_spec_t device_table[] = {
    {
        .id = UDR_KEYBOARD,
        .idt_slot =  33, // KEYBOARD_IDT 
        .port_regions = { { 0x60, 12 } }, 
        .port_regions_cnt = 1,
        .mem_regions = { },
        .mem_regions_cnt = 0
    },

    {
        .id = UDR_MOUSE,
        .idt_slot =  IDT_USER_START + 12, // MOUSE  
        .port_regions = { {0x60, 1}, {0x64, 1} },
        .port_regions_cnt = 2,
        .mem_regions = { },
        .mem_regions_cnt = 0
    },

    /* allows access to the console memory */
    {
        .id = UDR_CONSOLE,
        .idt_slot = UDR_NO_IDT,
        .port_regions = { },
        .port_regions_cnt = 0,
        .mem_regions = { { 0xb8000, 0x1000 } },
        .mem_regions_cnt = 1
    },
    
    { 
        .id = UDR_DEV_COM1,
        .idt_slot = 32 + 4,
        .port_regions = { { 0x3f8, 8 } },
        .port_regions_cnt = 1,
        .mem_regions = { },
        .mem_regions_cnt = 0,
    },

    {
        .id = UDR_DEV_COM2,
        .idt_slot = 32 + 3,
        .port_regions = { { 0x2f8, 8 } },
        .port_regions_cnt = 1,
        .mem_regions = { },
        .mem_regions_cnt = 0,
    }
};

const int device_table_entries = (sizeof device_table) / (sizeof device_table[0]);
