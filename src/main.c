#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "game.h"
#include "ui.h"

void signal_callback_handler(int signum)
{
    printf("         TERMINATED         \n");
    setBufferedInput(true);
    printf("\033[?25h\033[m");
    exit(signum);
}

int main(int argc, char *argv[])
{
    uint8_t board[SIZE][SIZE];
    uint8_t scheme = 0;
    uint32_t score = 0;
    int c;
    bool success;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            printf("Usage: 2048 [OPTION] | [MODE]\n");
            printf("Play the game 2048 in the console\n\n");
            printf("Options:\n");
            printf("  -h,  --help       Show this help message.\n");
            printf("  -v,  --version    Show version number.\n\n");
            printf("Modes:\n");
            printf("  bluered      Use a blue-to-red color scheme (requires 256-color terminal support).\n");
            printf("  blackwhite   The black-to-white color scheme (requires 256-color terminal support).\n");
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
        {
            printf("2048.c version %s\n", VERSION);
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[1], "blackwhite") == 0)
        {
            scheme = 1;
        }
        else if (strcmp(argv[1], "bluered") == 0)
        {
            scheme = 2;
        }
        else if (strcmp(argv[1], "test") == 0)
        {
            return testSucceed() ? EXIT_SUCCESS : EXIT_FAILURE;
        }
        else
        {
            printf("Invalid option: %s\n\nTry '%s --help' for more options.\n", argv[1], argv[0]);
            return EXIT_FAILURE;
        }
    }

    //清屏并隐藏光标
    printf("\033[?25l\033[2J");
    
    //接收到 Ctrl+C执行signal_callback_handler函数
    signal(SIGINT, signal_callback_handler);

    //初始化棋盘,设置随机元素
    initBoard(board);

    //设置终端无缓冲模式,键盘键入立刻被读取到,不用回车
    setBufferedInput(false);

    //打印棋盘
    drawBoard(board, scheme, score);
    
    while (true)
    {
        c = getchar();
        if (c == EOF)
        {
            puts("\nError! Cannot read keyboard input!");
            break;
        }
        switch (c)
        {
        case 52:
        case 97:
        case 104:
        case 68:
            success = moveLeft(board, &score);
            break;
        case 54:
        case 100:
        case 108:
        case 67:
            success = moveRight(board, &score);
            break;
        case 56:
        case 119:
        case 107:
        case 65:
            success = moveUp(board, &score);
            break;
        case 50:
        case 115:
        case 106:
        case 66:
            success = moveDown(board, &score);
            break;
        default:
            success = false;
        }
        if (success)
        {
            drawBoard(board, scheme, score);
            usleep(150 * 1000);
            addRandom(board);
            drawBoard(board, scheme, score);
            if (gameEnded(board))
            {
                printf("         GAME OVER          \n");
                break;
            }
        }
        if (c == 'q')
        {
            printf("        QUIT? (y/n)         \n");
            c = getchar();
            if (c == 'y')
            {
                break;
            }
            drawBoard(board, scheme, score);
        }
        if (c == 'r')
        {
            printf("       RESTART? (y/n)       \n");
            c = getchar();
            if (c == 'y')
            {
                initBoard(board);
                score = 0;
            }
            drawBoard(board, scheme, score);
        }
    }
    setBufferedInput(true);
    printf("\033[?25h\033[m");

    return EXIT_SUCCESS;
}