#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <stdbool.h>
#include "game.h"

void getColors(uint8_t value, uint8_t scheme, uint8_t *foreground, uint8_t *background);
uint8_t getDigitCount(uint32_t number);
void drawBoard(uint8_t board[SIZE][SIZE], uint8_t scheme, uint32_t score);
void setBufferedInput(bool enable);

#endif