
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <windows.h>
#include <stdio.h>

using std::cerr;
using std::endl;

class Graphics
{
public:
    Graphics(HANDLE hNewScreenBuffer, int logicalRows, int logicalCols) : hNewScreenBuffer(hNewScreenBuffer)
    {
        rows = logicalRows;
        cols = logicalCols * 2;

        buffer = new CHAR_INFO[rows * cols];
        clearBuffer();

        coordBufSize.Y = rows;
        coordBufSize.X = cols;

        coordBufCoord.X = 0;
        coordBufCoord.Y = 0;

        srctWriteRect.Top = 0;
        srctWriteRect.Left = 0;
        srctWriteRect.Bottom = rows - 1;
        srctWriteRect.Right = cols - 1;
    }
    void fillPixel(int x, int y)
    {
        buffer[y * cols + 2 * x].Char.AsciiChar = buffer[y * cols + 2 * x + 1].Char.AsciiChar = ' ';
        buffer[y * cols + 2 * x].Attributes = buffer[y * cols + 2 * x + 1].Attributes = BACKGROUND_GREEN;
    }

    void fillRect(int topLeftX, int topLeftY, int width, int height)
    {
        for (int x = topLeftX; x < topLeftX + width; x++)
        {
            for (int y = topLeftY; y < topLeftY + height; y++)
            {
                fillPixel(x, y);
            }
        }
    }

    void clearBuffer()
    {
        memset(buffer, 0, rows * cols * sizeof(buffer));
    }

    bool render()
    {
        // cerr << endl << "----------------" << endl;
        // for (int i=0; i<rows*cols; i++) {
        //     cerr << (int)buffer[i].Char.AsciiChar;
        // }
        // cerr << endl << "----------------" << endl;

        return WriteConsoleOutput(
            hNewScreenBuffer, // screen buffer to write to
            buffer,           // buffer to copy from
            coordBufSize,     // col-row size of chiBuffer
            coordBufCoord,    // top left src cell in chiBuffer
            &srctWriteRect);  // dest. screen buffer rectangle
    }

private:
    HANDLE hNewScreenBuffer;
    CHAR_INFO *buffer;
    COORD coordBufSize;
    COORD coordBufCoord;
    SMALL_RECT srctWriteRect;

    int rows;
    int cols;
};
#endif