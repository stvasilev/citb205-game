#include <windows.h>
#include <stdio.h>

#include "graphics.h"
#include <iostream>

int t = 10;
int x = 0;
int y = 3;
int dirX = +1;
int dirY = 0;

DWORD WINAPI render(LPVOID arg)
{
    Graphics *g = (Graphics *)arg;
    while (true)
    {
        g->clearBuffer();
        g->fillRect(x += dirX, y += dirY, 1, 1);
        g->render();
        Sleep(500);
    }
}

int main(void)
{
    HANDLE hStdin, hStdout, hNewScreenBuffer;
    BOOL fSuccess;

    INPUT_RECORD irec;
    DWORD cc;

    // Get a handle to the STDOUT screen buffer to copy from and
    // create a new screen buffer to copy to.

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hNewScreenBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | // read/write access
            GENERIC_WRITE,
        FILE_SHARE_READ |
            FILE_SHARE_WRITE,    // shared
        NULL,                    // default security attributes
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
        NULL);                   // reserved; must be NULL
    if (hStdout == INVALID_HANDLE_VALUE ||
        hNewScreenBuffer == INVALID_HANDLE_VALUE)
    {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    // Make the new screen buffer the active screen buffer.

    if (!SetConsoleActiveScreenBuffer(hNewScreenBuffer))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    Graphics g(hNewScreenBuffer, 40, 60);

    bool running = true;

    DWORD threadId;
    HANDLE thread = CreateThread(NULL, 0, render, &g, 0, &threadId);

    while (running)
    {
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

    // Restore the original active screen buffer.

    if (!SetConsoleActiveScreenBuffer(hStdout))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    return 0;
}