#pragma once

struct interrupt_frame
{
    int ip;
    int cs;
    int flags;
    int sp;
    int ss;
};

void default_interrupt_handler(struct interrupt_frame *frame);

#define DEFAULT_ISR(n)                                               \
void interrupt_handler##n(struct interrupt_frame *frame);      \

#include "default-isr-nums.inc"

#undef DEFAULT_ISR


