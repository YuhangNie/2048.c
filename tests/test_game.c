#include <stdio.h>
#include <assert.h>
#include "game.h"

int main(void)
{
    printf("Running 2048 game tests...\n");
    
    if (testSucceed())
    {
        printf("All tests passed!\n");
        return 0;
    }
    else
    {
        printf("Some tests failed!\n");
        return 1;
    }
}