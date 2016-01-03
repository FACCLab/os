/** @file ps2.h
 *  
 *  @brief Constants used to interact with PS/2 controller.
 *
 *  This contains an abbreviated list of the commands and responses used to
 *  configure an 8042 PS/2 controller. A more complete list can be found here:
 *
 *      http://www.computer-engineering.org/ps2keyboard/
 *      http://www.computer-engineering.org/ps2mouse/
 *
 *  @author John Gallagher (gallaghe)
 *  @bug No known bugs.
 */

#ifndef _PS2_H_
#define _PS2_H_

/* The I/O ports for the PS/2 controller. */
#define PS2_CMD_PORT            0x64
#define PS2_DATA_PORT           0x60

/* Commands for PS/2 Controller */
#define PS2_READ_CONFIG         0x20    /* Read the configuration byte */
#define PS2_WRITE_CONFIG        0x60    /* Store next byte to config byte */
#define PS2_PORT2_DIS           0xA7    /* Disable second port */
#define PS2_PORT2_EN            0xA8    /* Enable second port */
#define PS2_PORT2_TEST          0xA9    /* Perform test of second port */
#define PS2_SELF_TEST           0xAA    /* Perform self-test of controller */
#define PS2_PORT1_TEST          0xAB    /* Perform test of first port */
#define PS2_PORT1_DIS           0xAD    /* Disable first port */
#define PS2_PORT1_EN            0xAE    /* Enable first port */
#define PS2_PORT2_WRITE         0xD4    /* Send next byte to device on port 2 */

/* Responses from PS/2 Controller */
#define PS2_PORT_TEST_OK        0x00
#define PS2_SELF_TEST_OK        0x55

/* The flags that make up the PS2 Controller configuration byte. Some of the
 * bits have undefined values which may not be modified, so the byte must
 * read, modified, and then written. Bit 4 can be set / cleared by issuing the
 * PS2_PORT1_DIS / PS2_PORT1_EN commands. Likewise, bit 5 can be set / cleared
 * by issuing the PS2_PORT2_DIS / PS2_PORT2_EN, if port 2 exits. */
#define PS2_PORT1_INT_EN        (1<<0)  /* Set to enable kbd ints on irq1 */
#define PS2_PORT2_INT_EN        (1<<1)  /* Set to enable mouse ints on irq12 */
#define PS2_PORT1_IS_DIS        (1<<4)  /* When set, 1st port is disabled. */
#define PS2_PORT2_IS_DIS        (1<<5)  /* When set, 2nd port is disabled. */
#define PS2_PORT1_TRANSLATION   (1<<6)  /* Translate scancodes to set 1 */

/* Commands for a keyboard connected to PS/2 port 1 */
#define KBD_SCAN_EN             0xF4    /* Enable keyboard scanning */
#define KBD_SCAN_DIS            0xF5    /* Disable keyboard scanning */
#define KBD_RESET               0xFF

/* Commands for a mouse connected to PS/2 port 2 */
#define MOUSE_RESET             0xFF
#define MOUSE_SET_DFLT          0xF6    /* Restore default settings */
#define MOUSE_DIS_REPORT        0xF5    /* Disable data reporting */
#define MOUSE_EN_REPORT         0xF4    /* Enable data reporting */
#define MOUSE_SET_RATE          0xF3    /* Set sample rate */
#define MOUSE_GET_ID            0xF2    /* Get device ID */
#define MOUSE_SET_SCALE_2_1     0xE7    /* Set scaling 2:1 */
#define MOUSE_SET_SCALE_1_1     0xE6    /* Set scaling 1:1 */
#define MOUSE_SET_RES           0xE8    /* Set device resolution */
#define MOUSE_SET_STREAM        0xEA    /* Set stream mode */
#define MOUSE_SET_RATE          0xF3    /* Set sample rate */

/* Both the keyboard and mouse will send 0xFA upon recieving a valid
 * command. */
#define KBD_MOUSE_ACK           0xFA

/* The flags that make up the first byte of a mouse event packet. The X sign
 * bit, left shifted by 8, can be or'd with the second byte of the packet to
 * form a 9-bit two's complement representation of the relative movement of
 * the mouse. Likewise for for the Y sign bit and the third byte. If the
 * movement is too great to be fit in this format, the appropriate overflow
 * bit is set. */
#define MOUSE_LEFT_BTN          (1<<0)  /* Set when button is pressed */
#define MOUSE_RIGHT_BTN         (1<<1)
#define MOUSE_MIDDLE_BTN        (1<<2)
#define MOUSE_IS_FIRST_BYTE     (1<<3)  /* Always set in first byte in packet */
#define MOUSE_X_SIGN            (1<<4)
#define MOUSE_Y_SIGN            (1<<5)
#define MOUSE_X_OVERFLOW        (1<<6)
#define MOUSE_Y_OVERFLOW        (1<<7)

#endif  /* _PS2_H_ */
