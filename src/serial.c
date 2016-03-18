// Copyright (C) 2015-2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "serial.h"
#include "irq.h"
#include <stdint.h>
#include <errno.h>
#include "socket.h"
#include "panic.h"

// Implementation of the primitive PC serial transport.

// more information about serial ports than you could ever want:
// http://retired.beyondlogic.org/serial/serial.htm

// another informative resource:
// https://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming

// The UART has six register addresses. The meaning depends on the state of
// the DLAB flag and whether you are reading from or writing to the register.
#define RBR 0	// Receive buffer: DLAB=0, input
#define THR 0	// Transmit holding: DLAB=0, output
#define DLL 0	// Divisor latch low: DLAB=1
#define IER 1	// Interrupt enable: DLAB=0
#define DLH 1	// Divisor latch high: DLAB=1
#define IIR 2	// Interrupt ID, input
#define FCR 2	// FIFO control, output
#define LCR 3	// Line control and DLAB switch
#define MCR 4	// Modem control
#define LSR 5	// Line status
#define MSR 6	// Modem status

// There are four events that can provide interrupts.
// OR these flags together to indicate the interrupts you want.
#define IER_RX_DATA 1		// Data ready
#define IER_THRE 2			// Transfer holding register empty
#define IER_RX_STATUS 4 	// PE, FE, overrun, BREAK
#define IER_MODEM_STATUS 8	// CTS, DSR, RI, DCD

// The low three bits of the IIR identify the highest-priority interrupt
// event that has occurred.
#define IIR_MASK 7
#define IIR_NONE 1
#define IIR_MODEM_STATUS 0
#define IIR_THRE 2
#define IIR_RX_DATA 4
#define IIR_RX_STATUS 6

// The FCR configures the FIFO options.
#define FIFO_ENABLE 0x01
#define FIFO_CLEAR_RX 0x02
#define FIFO_CLEAR_TX 0x04
#define FIFO_CLEAR_ALL 0x06
#define FIFO_DMA_MODE 0x08	// switches from mode 0 to mode 1?
#define FIFO_64 0x10	// 64-byte FIFO instead of 16
// Pick one:
#define FIFO_TRIGGER_1 0x00
#define FIFO_TRIGGER_4 0x40
#define FIFO_TRIGGER_8 0x80
#define FIFO_TRIGGER_14 0xC0

// The line control register configures the serial protocol.
#define LCR_DLAB 0x80	// switch divisor registers on or off
#define LCR_BREAK 0x40	// continuously write out spaces
#define LCR_PARITY_MODE 0x08	// use a parity mode
#define LCR_PARITY_STICKY 0x20	// sticky parity
#define LCR_PARITY_EVEN 0x10	// otherwise odd
#define LCR_EXTRA_STOP 0x40	// 1.5/2 stop bits mode, otherwise 1 stop bit
#define LCR_WORD_8 0x03 // use this
#define LCR_WORD_7 0x02 // why would you do this?
#define LCR_WORD_6 0x01 // pain
#define LCR_WORD_5 0x00 // huh?

// The line status register tells us what the port is up to.
#define LSR_RX_READY 0x01
#define LSR_RX_OVERRUN 0x02
#define LSR_PARITY_ERROR 0x04
#define LSR_FRAMING_ERROR 0x08
#define LSR_BREAK 0x10
#define LSR_TX_READY 0x20
#define LSR_TX_STALL 0x40
#define LSR_FIFO_ERROR 0x80

// The modem status register describes communication signalling states.
#define MSR_DCD 0x80	// carrier detect
#define MSR_RI 0x40 // ring indicator
#define MSR_DSR 0x20	// data set ready
#define MSR_CTS 0x10	// clear to send
#define MSR_X_DCD 0x08	// change in DCD
#define MSR_X_RI 0x04	// low RI went high
#define MSR_X_DSR 0x02 // change in DSR
#define MSR_X_CTS 0x01 // change in CTS

// The modem control register manages flow control.
#define MCR_DTR 0x01	// data terminal is ready for input
#define MCR_RTS 0x02	// please send data
#define MCR_AUX1 0x04
#define MCR_AUX2 0x08
#define MCR_LOOPBACK 0x10
#define MCR_AUTOFLOW 0x20

static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile("outb %0, %1": :"a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile("inb %1, %0": "=a"(ret): "Nd"(port));
	return ret;
}

struct serial {
	unsigned port;
	unsigned irq;
	int streamid;
};

struct serial COM1 = { .port = 0x3F8, .irq = 4 };
struct serial COM2 = { .port = 0x2F8, .irq = 3 };
struct serial COM3 = { .port = 0x3E8, .irq = 4 };
struct serial COM4 = { .port = 0x2E8, .irq = 3 };

static int serial_read(void *ref, void *buf, unsigned bytes);
static int serial_write(void *ref, const void *buf, unsigned bytes);
static void serial_close(void *ref);

static struct iops serial_ops = {
	.read = serial_read,
	.write = serial_write,
	.close = serial_close,
};

static void serial_irq(void *ref)
{
	struct serial *serial = (struct serial*)ref;
	// See what's up with this port. Why did an interrupt occur?
	uint8_t iir = inb(serial->port + IIR);
	switch (iir & IIR_MASK) {
		case IIR_NONE: return; // well that was weird
		case IIR_MODEM_STATUS: {
			// we don't really care, but we need to clear the condition,
			// so we'll read from the status register
        	inb(serial->port + MSR);
		} break;
		case IIR_THRE: /* TX ready */ break;
		case IIR_RX_DATA: /* RX ready */ break;
		case IIR_RX_STATUS: {
			// nothing we can usefully do just yet, but we need to clear
			// the condition, so we'll read from the status register
        	inb(serial->port + LSR);
		} break;
	}
}

int _serial_open(struct serial *serial)
{
	if (serial->streamid) return -EISCONN;
	// Switch DLAB on and set the speed to 115200.
	outb(serial->port + LCR, LCR_DLAB);
	outb(serial->port + DLL, 0x01);
	outb(serial->port + DLH, 0x00);
	// Switch DLAB off and configure 8N1 mode.
	outb(serial->port + LCR, LCR_WORD_8);
	// Enable FIFO mode and clear buffers.
	outb(serial->port + FCR, FIFO_ENABLE|FIFO_CLEAR_ALL|FIFO_TRIGGER_14);
	// Add ourselves to the notify queue for the port's IRQ.
	_irq_listen(serial->irq, serial, &serial_irq);
	// Enable interrupts so we don't have to waste time polling.
	// We want to know when data is ready to send or to receive.
	outb(serial->port + IER, IER_RX_DATA|IER_THRE);
	// Create and return a stream ID so system calls can refer to this.
	serial->streamid = open(serial, &serial_ops);
}

static int serial_write(void *ref, const void *buf, unsigned bytes)
{
	struct serial *serial = (struct serial*)ref;
	const char *p = buf;
	const char *end = buf + bytes;
	unsigned lsr_addr = serial->port + LSR;
	unsigned thr_addr = serial->port + THR;
	// Write into the port until our buffer is empty or its buffer is full.
	while (p < end) {
		if (inb(lsr_addr) & LSR_TX_READY) {
			// Add this byte to the FIFO.
			outb(thr_addr, *p++);
		} else {
			// Wastefully burn CPU cycles on blocking IO.
		}
	}
	// Return the number of bytes we read.
	return p - (char*)buf;
}

static int serial_read(void *ref, void *buf, unsigned capacity)
{
	struct serial *serial = (struct serial*)ref;
	char *p = buf;
	char *end = buf + capacity;
	unsigned lsr_addr = serial->port + LSR;
	unsigned rbr_addr = serial->port + RBR;
	// Read bytes from this port until we empty it or run out of buffer.
	while (p < end) {
		if (inb(lsr_addr) & LSR_RX_READY) {
			// Copy in the next byte from the FIFO.
			*p++ = inb(rbr_addr);
		} else {
			// Burn CPU cycles for no good reason blocking on IO.
		}
	}
	return p - (char*)buf;
}

static void serial_close(void *ref)
{
	struct serial *serial = (struct serial*)ref;
	assert(serial->streamid != 0);
	serial->streamid = 0;
	// Disable interrupts.
	outb(serial->port + IER, 0);
	// Detach from the ISR.
	_irq_ignore(serial->irq);
}



