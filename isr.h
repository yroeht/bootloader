#pragma once

struct interrupt_frame
{
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



