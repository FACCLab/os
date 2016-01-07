/* @file udriv_kern.h
 * 
 * @brief Contains definitions to help the kernel identify and maintain
 * permissions for device I/O 
 *
 * @author Amanda M. Watson (amwatson)
 * @bug No known bugs 
 */ 

#ifndef _UDRIV_KERN_
#define _UDRIV_KERN_

#include <x86/idt.h>
#include <udriv_registry.h> // driv_id_t
#include <stdint.h>

// lowest possible IDT entry in table (IDT_USER_START will be reserved for the timer)
#define UDR_IDT_MIN (IDT_USER_START + 1)
// highest possible IDTentry in table 
#define UDR_IDT_MAX (IDT_ENTS  - 1)

// "This table entry does not generate interrupts".
#define UDR_NO_IDT 0 

/* device table limits */ 

#define MAX_IO_REGIONS_PER_ENTRY 4
#define MAX_MEM_REGIONS_PER_ENTRY 4

// max number of drivers in table
#define DRIVERS_MAX_NUM 128

// defines a region (port or MMIO)
typedef struct {
	uint32_t base; // base address
	uint32_t len; // length of region from base
} udrv_region_t;

// entry for a device in device table
typedef struct {
	
	// The device name.
	driv_id_t id;

	// IDT slot via which interrupts arrive (presently 32 + IRQ, see x86/interrupts.c)
	// Also used by udrv_wait()
	uint32_t idt_slot;

	// The regions in the "Port IO" address space that the udriv
	// is allowed to manipulate using calls to udriv_inb/udriv_outb.
	udrv_region_t port_regions[MAX_IO_REGIONS_PER_ENTRY];
	uint32_t port_regions_cnt;

	// The regions of memory the USD may map into its address space.
	udrv_region_t mem_regions[MAX_MEM_REGIONS_PER_ENTRY];
	uint32_t mem_regions_cnt;

} dev_spec_t;

// kernel-side table describing available devices
extern const dev_spec_t device_table[]; 
extern const int device_table_entries;

#endif /* _UDRIV_KERN_ */
