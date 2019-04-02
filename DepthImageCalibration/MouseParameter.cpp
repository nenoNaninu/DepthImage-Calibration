#include "pch.h"
#include "MouseParameter.h"

void mFunc(int event, int x, int y, int flags, void *param)
{
    MouseParameter* mouseParamPtr = static_cast<MouseParameter*>(param);
    mouseParamPtr->x = x;
    mouseParamPtr->y = y;
    mouseParamPtr->event = event;
    mouseParamPtr->flags = flags;
}
