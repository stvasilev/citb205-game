#include <iostream>
#include "graphics.h"
#include "generator.h"
#include "point.h"
#include "read_input.h"
#include "buffer_error.h"

auto num = int_generator(40, 60); 

Point food(num() % 60, num() % 40); //first food point

DWORD WINAPI render(LPVOID arg)
{
    Graphics *g = (Graphics *)arg;
    while (true)
    {

        g->clearBuffer();

        for (auto segment : snake)
        {
            g->fillPixel(segment.x, segment.y, BACKGROUND_GREEN);
        }

        g->fillPixel(food.x, food.y, BACKGROUND_RED);

        int x = snake.front().x + dirX;
        int y = snake.front().y + dirY;

        //if snake eats food generate new food
        if (snake.front().x == food.x && snake.front().y == food.y)
        {
            food.x = num() % 60;
            food.y = num() % 40;
        }
        else
        {
            snake.pop_back();
        }
        snake.push_front(Point(x, y));

        g->render();
        Sleep(150);
    }
}

int main(void)
{

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
        return buffer_error();
    }

    // Make the new screen buffer the active screen buffer.

    if (!SetConsoleActiveScreenBuffer(hNewScreenBuffer))
    {
        return buffer_error();
    }

    snake.push_back(Point(1, 1));
    snake.push_back(Point(2, 1));
    snake.push_back(Point(3, 1));

    Graphics g(hNewScreenBuffer, 40, 60);

    DWORD threadId;
    HANDLE thread = CreateThread(NULL, 0, render, &g, 0, &threadId);

    while (running)
    {
        readInput();
    }

    // Restore the original active screen buffer.

    if (!SetConsoleActiveScreenBuffer(hStdout))
    {
        return buffer_error();
    }

    return 0;
}