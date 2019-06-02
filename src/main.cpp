#include <windows.h>
#include <stdio.h>
#include <iostream>

class Graphics
{
public:
    Graphics(HANDLE hNewScreenBuffer, int logicalRows, int logicalCols) : hNewScreenBuffer(hNewScreenBuffer)
    {
        rows = logicalRows;
        cols = logicalCols * 2;
        buffer = new CHAR_INFO[rows * cols]{};
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
        buffer[y * cols + 2*x].Char.AsciiChar = buffer[y * cols + 2*x+1].Char.AsciiChar = ' ';
        buffer[y * cols + 2*x].Attributes = buffer[y * cols + 2*x+1].Attributes = BACKGROUND_GREEN;
    }

    void fillRect(int topLeftX, int topLeftY, int width, int height)
    {
        for (int x = topLeftX; x < topLeftX+width; x++) {
            for (int y = topLeftY; y < topLeftY + height; y++) {
                fillPixel(x, y);
            }
        }
    }

    bool render()
    {
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
int main(void)
{
    HANDLE hStdout, hNewScreenBuffer;
    BOOL fSuccess;

    // Get a handle to the STDOUT screen buffer to copy from and
    // create a new screen buffer to copy to.

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
    
    // g.fillPixel(5, 3);
    // g.fillRect(5, 3, 7, 4);
    g.fillCircle(10, 10, 5);


    fSuccess = g.render();
    if (!fSuccess)
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    Sleep(5000);

    // Restore the original active screen buffer.

    if (!SetConsoleActiveScreenBuffer(hStdout))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    return 0;
}