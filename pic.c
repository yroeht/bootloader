#include "print.h"


enum pic_irq_master
{
	pic_irq_system_timer,
	pic_irq_keyboard,
	pic_irq_cascade,
	pic_irq_serial_port2_,
	pic_irq_serial_port1,
	pic_irq_parallel_port3,
	pic_irq_floppy,
	pic_irq_parallel_port1
};

enum pic_irq_slave
{
	pic_irq_realtime_clock,
	pic_irq_acpi,
	pic_irq_open1,
	pic_irq_open2,
	pic_irq_mouse,
	pic_irq_coprocessor,
	pic_irq_ata_primary,
	pic_irq_ata_secondaty
};

const char master_irq_offset = 0x40;
const char slave_irq_offset = 0x10;

enum port
{
	pic_port_master_control = 0x20,
	pic_port_master_data = 0x21,
	pic_port_slave_control = 0xa0,
	pic_port_slave_data = 0xa1
};

static inline void outb(short port, unsigned char b)
{
	asm volatile("outb %0, %1" : : "a"(b), "Nd"(port));
}
static inline char inb(short port)
{
	char ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void pic_init(void)
{
	// icw1
	outb(pic_port_master_control, 0x11);
	outb(pic_port_slave_control, 0x11);

	// icw2
	outb(pic_port_master_data, master_irq_offset);
	outb(pic_port_slave_data, slave_irq_offset);

	// icw3
	outb(pic_port_master_data, 0x01);
	outb(pic_port_slave_data, 0x01);

	// icw4
	outb(pic_port_master_data, 0x01);
	outb(pic_port_slave_data, 0x01);

	// ocw1
	char irq_mask = ~0; //inb(pic_port_master_data);
	irq_mask &= ~(1 << pic_irq_keyboard);
	//irq_mask &= ~(1 << pic_irq_system_timer);
	outb(pic_port_master_data, irq_mask);
}

void pic_ack(void)
{
	outb(pic_port_master_control, 0x20);
	outb(pic_port_slave_control, 0x20);
}
