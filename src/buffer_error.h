#ifndef BUFFER_ERROR_H
#define BUFFER_ERROR_H

#include <iostream>
#include <windows.h>

int buffer_error()
{
    printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
    return 1;
}

#endif