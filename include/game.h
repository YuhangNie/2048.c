#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

#define SIZE 4
#define VERSION "1.0.3"

typedef struct
{
    uint8_t board[SIZE][SIZE];
    uint32_t score;
} Game;

void initBoard(uint8_t board[SIZE][SIZE]);
void addRandom(uint8_t board[SIZE][SIZE]);
bool moveUp(uint8_t board[SIZE][SIZE], uint32_t *score);
bool moveLeft(uint8_t board[SIZE][SIZE], uint32_t *score);
bool moveDown(uint8_t board[SIZE][SIZE], uint32_t *score);
bool moveRight(uint8_t board[SIZE][SIZE], uint32_t *score);
bool gameEnded(uint8_t board[SIZE][SIZE]);
uint8_t countEmpty(uint8_t board[SIZE][SIZE]);
bool testSucceed(void);

bool slideArray(uint8_t array[SIZE], uint32_t *score);
uint8_t findTarget(uint8_t array[SIZE], uint8_t x, uint8_t stop);
void rotateBoard(uint8_t board[SIZE][SIZE]);
bool findPairDown(uint8_t board[SIZE][SIZE]);

#endif