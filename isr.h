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
