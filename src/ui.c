#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "ui.h"

void getColors(uint8_t value, uint8_t scheme, uint8_t *foreground, uint8_t *background)
{
    uint8_t original[] = {8, 255, 1, 255, 2, 255, 3, 255, 4, 255, 5, 255, 6, 255, 7, 255, 9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 255, 0, 255, 0};
    uint8_t blackwhite[] = {232, 255, 234, 255, 236, 255, 238, 255, 240, 255, 242, 255, 244, 255, 246, 0, 248, 0, 249, 0, 250, 0, 251, 0, 252, 0, 253, 0, 254, 0, 255, 0};
    uint8_t bluered[] = {235, 255, 63, 255, 57, 255, 93, 255, 129, 255, 165, 255, 201, 255, 200, 255, 199, 255, 198, 255, 197, 255, 196, 255, 196, 255, 196, 255, 196, 255, 196, 255};
    uint8_t *schemes[] = {original, blackwhite, bluered};
    *foreground = *(schemes[scheme] + (1 + value * 2) % sizeof(original));
    *background = *(schemes[scheme] + (0 + value * 2) % sizeof(original));
}

uint8_t getDigitCount(uint32_t number)
{
    uint8_t count = 0;
    do
    {
        number /= 10;
        count += 1;
    } while (number);
    return count;
}

void drawBoard(uint8_t board[SIZE][SIZE], uint8_t scheme, uint32_t score)
{
    uint8_t x, y, fg, bg;
    printf("\033[H");
    printf("2048.c %17u pts\n\n", score);
    for (y = 0; y < SIZE; y++)
    {
        for (x = 0; x < SIZE; x++)
        {
            getColors(board[x][y], scheme, &fg, &bg);
            printf("\033[38;5;%u;48;5;%um", fg, bg);
            printf("       ");
            printf("\033[m");
        }
        printf("\n");
        for (x = 0; x < SIZE; x++)
        {
            getColors(board[x][y], scheme, &fg, &bg);
            printf("\033[38;5;%u;48;5;%um", fg, bg);
            if (board[x][y] != 0)
            {
                uint32_t number = 1 << board[x][y];
                uint8_t t = 7 - getDigitCount(number);
                printf("%*s%u%*s", t - t / 2, "", number, t / 2, "");
            }
            else
            {
                printf("   ·   ");
            }
            printf("\033[m");
        }
        printf("\n");
        for (x = 0; x < SIZE; x++)
        {
            getColors(board[x][y], scheme, &fg, &bg);
            printf("\033[38;5;%u;48;5;%um", fg, bg);
            printf("       ");
            printf("\033[m");
        }
        printf("\n");
    }
    printf("\n");
    printf("        ←,↑,→,↓ or q        \n");
    printf("\033[A");
}

void setBufferedInput(bool enable)
{
    static bool enabled = true;
    static struct termios old;
    struct termios new;

    if (enable && !enabled)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        enabled = true;
    }
    else if (!enable && enabled)
    {
        tcgetattr(STDIN_FILENO, &new);
        old = new;
        new.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new);
        enabled = false;
    }
}