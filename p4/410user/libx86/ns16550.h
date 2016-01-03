/** @file ns16550.h
 *  
 *  @brief Constants used to interact with NS16550 UART
 *
 *  This contains an abbreviated list of the commands and responses used to
 *  configure a National Semiconductor 16550 (or compatible) UART.
 *
 *  @author Thomas H Chittenden (tchitten)
 */

#ifndef _NS16550_H_
#define _NS16550_H_

#define COM1_IO_BASE 0x3f8
#define COM2_IO_BASE 0x2f8
#define COM3_IO_BASE 0x3e8
#define COM4_IO_BASE 0x2e8
#define UART_CLOCK 115200
#define BIT(n) (1 << (n))
#define GET_BYTE(b, i) (((b) >> (8*(i))) & 0xff)

// Interrupt Enable Register flags.
typedef enum {
    IER_LOW_POWER_MODE_EN = BIT(5),
    IER_SLEEP_MODE_EN = BIT(4),
    IER_MODEM_STATUS_INT_EN = BIT(3),
    IER_RECV_LINE_STAT_INT_EN = BIT(2),
    IER_TX_EMPTY_INT_EN = BIT(1),
    IER_RX_FULL_INT_EN = BIT(0),
} reg_ier_flags;

// Interrupt Identification Register flags.
typedef enum {
    IIR_INT_PENDING_BIT = BIT(0),
    IIR_INT_PENDING_NONE = BIT(0),
    IIR_INT_PENDING_SOME = 0,
    IIR_INT_TYPE = BIT(2) | BIT(1),
    IIR_INT_TYPE_MODEM_STATUS = 0,
    IIR_INT_TYPE_TX = BIT(1),
    IIR_INT_TYPE_RX = BIT(2),
    IIR_INT_TYPE_RECV_LINE_STATUS = BIT(1) | BIT(2),
} reg_iir_flags;

// Line Control Register flags.
typedef enum {
    LCR_WORD_LENGTH_TYPE = BIT(1) | BIT(0),
    LCR_WORD_LENGTH_TYPE_8 = BIT(1) | BIT(0),
    LCR_WORD_LENGTH_TYPE_7 = BIT(1),
    LCR_WORD_LENGTH_TYPE_6 = BIT(0),
    LCR_WORD_LENGTH_TYPE_5 = 0,
    LCR_STOP_BIT_TYPE = BIT(2),
    LCR_STOP_BIT_TWO = BIT(2),
    LCR_STOP_BIT_ONE = 0,
    LCR_PARITY_TYPE = BIT(5) | BIT(4) | BIT(3),
    LCR_PARITY_LOW = BIT(5) | BIT(4) | BIT(3),
    LCR_PARITY_HIGH = BIT(5) | BIT(3),
    LCR_PARITY_EVEN = BIT(4) | BIT(3),
    LCR_PARITY_ODD = BIT(3),
    LCR_PARITY_NONE = 0,
    LCR_BRK_EN = BIT(6),
    LCR_DLAB = BIT(7),
} reg_lcr_flags;

// Line Status Register flags.
typedef enum {
    LSR_DATA_READY = BIT(0),
    LSR_OVERRUN_ERR = BIT(1),
    LSR_PARITY_ERR = BIT(2),
    LSR_FRAMING_ERR = BIT(3),
    LSR_BREAK_INT = BIT(4),
    LSR_TX_EMPTY = BIT(5),
    LSR_RX_FULL = BIT(6),
    LSR_RECV_FIFO_ERR = BIT(7),
} reg_lsr_flags;

typedef enum {
    PORT_COM1,
    PORT_COM2,
    PORT_COM3,
    PORT_COM4,
} port_t;

typedef enum {
    CONF_8N1 = LCR_WORD_LENGTH_TYPE_8 | LCR_PARITY_NONE | LCR_STOP_BIT_ONE
} conf_t;

typedef enum {
    REG_DATA = 0,
    REG_INT_EN = 1,
    REG_BAUD_LSB = 0, /* DLAB = 1 */
    REG_BAUD_MSB = 1, /* DLAB = 1 */
    REG_FIFO_CTL = 2,
    REG_INT_ID = 2,
    REG_LINE_CNTL = 3,
    REG_MOD_CNTL = 4,
    REG_LINE_STAT = 5,
    REG_MOD_STAT = 6,
    REG_SCRATCH = 7
} reg_t;

#endif /* _NS16550_H_ */
