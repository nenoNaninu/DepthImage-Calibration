#pragma once

struct MouseParameter
{
    unsigned int x;
    unsigned int y;
    int event;
    int flags;
};

void mFunc(int event, int x, int y, int flags, void *param);