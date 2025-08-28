#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"

uint8_t findTarget(uint8_t array[SIZE], uint8_t x, uint8_t stop)
{
    uint8_t t;
    if (x == 0)
    {
        return x;
    }
    for (t = x - 1;; t--)
    {
        if (array[t] != 0)
        {
            if (array[t] != array[x])
            {
                return t + 1;
            }
            return t;
        }
        else
        {
            if (t == stop)
            {
                return t;
            }
        }
    }
    return x;
}

bool slideArray(uint8_t array[SIZE], uint32_t *score)
{
    bool success = false;
    uint8_t x, t, stop = 0;

    for (x = 0; x < SIZE; x++)
    {
        if (array[x] != 0)
        {
            t = findTarget(array, x, stop);
            if (t != x)
            {
                if (array[t] == 0)
                {
                    array[t] = array[x];
                }
                else if (array[t] == array[x])
                {
                    array[t]++;
                    *score += 1 << array[t];
                    stop = t + 1;
                }
                array[x] = 0;
                success = true;
            }
        }
    }
    return success;
}

void rotateBoard(uint8_t board[SIZE][SIZE])
{
    uint8_t i, j, n = SIZE;
    uint8_t tmp;
    for (i = 0; i < n / 2; i++)
    {
        for (j = i; j < n - i - 1; j++)
        {
            tmp = board[i][j];
            board[i][j] = board[j][n - i - 1];
            board[j][n - i - 1] = board[n - i - 1][n - j - 1];
            board[n - i - 1][n - j - 1] = board[n - j - 1][i];
            board[n - j - 1][i] = tmp;
        }
    }
}

bool moveUp(uint8_t board[SIZE][SIZE], uint32_t *score)
{
    bool success = false;
    uint8_t x;
    for (x = 0; x < SIZE; x++)
    {
        success |= slideArray(board[x], score);
    }
    return success;
}

bool moveLeft(uint8_t board[SIZE][SIZE], uint32_t *score)
{
    bool success;
    rotateBoard(board);
    success = moveUp(board, score);
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return success;
}

bool moveDown(uint8_t board[SIZE][SIZE], uint32_t *score)
{
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp(board, score);
    rotateBoard(board);
    rotateBoard(board);
    return success;
}

bool moveRight(uint8_t board[SIZE][SIZE], uint32_t *score)
{
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp(board, score);
    rotateBoard(board);
    return success;
}

bool findPairDown(uint8_t board[SIZE][SIZE])
{
    bool success = false;
    uint8_t x, y;
    for (x = 0; x < SIZE; x++)
    {
        for (y = 0; y < SIZE - 1; y++)
        {
            if (board[x][y] == board[x][y + 1])
                return true;
        }
    }
    return success;
}

uint8_t countEmpty(uint8_t board[SIZE][SIZE])
{
    uint8_t x, y;
    uint8_t count = 0;
    for (x = 0; x < SIZE; x++)
    {
        for (y = 0; y < SIZE; y++)
        {
            if (board[x][y] == 0)
            {
                count++;
            }
        }
    }
    return count;
}

bool gameEnded(uint8_t board[SIZE][SIZE])
{
    bool ended = true;
    if (countEmpty(board) > 0)
        return false;
    if (findPairDown(board))
        return false;
    rotateBoard(board);
    if (findPairDown(board))
        ended = false;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return ended;
}

void addRandom(uint8_t board[SIZE][SIZE])
{
    static bool initialized = false;
    uint8_t x, y;
    uint8_t r, len = 0;
    uint8_t n, list[SIZE * SIZE][2];

    if (!initialized)
    {
        srand(time(NULL));
        initialized = true;
    }

    for (x = 0; x < SIZE; x++)
    {
        for (y = 0; y < SIZE; y++)
        {
            if (board[x][y] == 0)
            {
                list[len][0] = x;
                list[len][1] = y;
                len++;
            }
        }
    }

    if (len > 0)
    {
        r = rand() % len;
        x = list[r][0];
        y = list[r][1];
        n = (rand() % 10) / 9 + 1;
        board[x][y] = n;
    }
}

void initBoard(uint8_t board[SIZE][SIZE])
{
    uint8_t x, y;
    for (x = 0; x < SIZE; x++)
    {
        for (y = 0; y < SIZE; y++)
        {
            board[x][y] = 0;
        }
    }
    addRandom(board);
    addRandom(board);
}

bool testSucceed(void)
{
    uint8_t array[SIZE];
    uint8_t data[] = {
        0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 1, 1, 2, 0, 0, 0, 4,
        0, 1, 0, 1, 2, 0, 0, 0, 4,
        1, 0, 0, 1, 2, 0, 0, 0, 4,
        1, 0, 1, 0, 2, 0, 0, 0, 4,
        1, 1, 1, 0, 2, 1, 0, 0, 4,
        1, 0, 1, 1, 2, 1, 0, 0, 4,
        1, 1, 0, 1, 2, 1, 0, 0, 4,
        1, 1, 1, 1, 2, 2, 0, 0, 8,
        2, 2, 1, 1, 3, 2, 0, 0, 12,
        1, 1, 2, 2, 2, 3, 0, 0, 12,
        3, 0, 1, 1, 3, 2, 0, 0, 4,
        2, 0, 1, 1, 2, 2, 0, 0, 4};
    uint8_t *in, *out, *points;
    uint8_t t, tests;
    uint8_t i;
    bool success = true;
    uint32_t score;

    tests = (sizeof(data) / sizeof(data[0])) / (2 * SIZE + 1);
    for (t = 0; t < tests; t++)
    {
        in = data + t * (2 * SIZE + 1);
        out = in + SIZE;
        points = in + 2 * SIZE;
        for (i = 0; i < SIZE; i++)
        {
            array[i] = in[i];
        }
        score = 0;
        slideArray(array, &score);
        for (i = 0; i < SIZE; i++)
        {
            if (array[i] != out[i])
            {
                success = false;
            }
        }
        if (score != *points)
        {
            success = false;
        }
        if (success == false)
        {
            printf("Test failed:\n");
            for (i = 0; i < SIZE; i++)
            {
                printf("%u ", in[i]);
            }
            printf("=> ");
            for (i = 0; i < SIZE; i++)
            {
                printf("%u ", array[i]);
            }
            printf("(%u points) expected ", score);
            for (i = 0; i < SIZE; i++)
            {
                printf("%u ", out[i]);
            }
            printf("(%u points)\n", *points);
            break;
        }
    }
    if (success)
    {
        printf("All %u tests executed successfully\n", tests);
    }
    return success;
}