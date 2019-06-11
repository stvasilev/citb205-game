#ifndef READ_INPUT_H
#define READ_INPUT_H

#include <Windows.h>

bool running = true;

int dirX = +1; //2 cmd blocks = 1 square
int dirY = 0;

HANDLE hStdin, hStdout, hNewScreenBuffer;
BOOL fSuccess;

INPUT_RECORD irec;
DWORD cc;

void readInput() {
        ReadConsoleInput(hStdin, &irec, 1, &cc);
        if (irec.EventType == KEY_EVENT && ((KEY_EVENT_RECORD &)irec.Event).bKeyDown)
        {
            KEY_EVENT_RECORD krec = (KEY_EVENT_RECORD &)irec.Event;

            switch (krec.wVirtualKeyCode)
            {
            case VK_DOWN:
                dirX = 0;
                dirY = +1;
                break;
            case VK_UP:
                dirX = 0;
                dirY = -1;
                break;
            case VK_LEFT:
                dirX = -1;
                dirY = 0;
                break;
            case VK_RIGHT:
                dirX = 1;
                dirY = 0;
                break;
            case VK_ESCAPE:
                running = false;
                break;
            }
        }
}

#endif