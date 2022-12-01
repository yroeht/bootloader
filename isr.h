#pragma once

struct interrupt_frame
{
    int err;
    int ip;
    int cs;
    int flags;
    int sp;
    int ss;
};

#define ISR(n, str, has_error, is_excn)                        \
void interrupt_handler##n(struct interrupt_frame *frame);      \

#include "isr-list.inc"

#undef ISR

void default_interrupt_handler(struct interrupt_frame *frame);

void keyboard_handler(struct interrupt_frame *frame);

void syscall_handler(struct interrupt_frame *frame);

void page_fault_handler(struct interrupt_frame *frame);
