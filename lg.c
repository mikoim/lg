#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "lg.h"

#define random(MIN, MAX) MIN + (int)(xor128()*(MAX-MIN+1.0)/(1.0+UINT32_MAX))

typedef struct tagRectangular
{
	int x;
	int y;
} Rectangular;

uint32_t xor128seed;

uint32_t xor128(void) {
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    uint32_t t;
    
    t = x ^ (x << 11);
    x = y; y = z; z = xor128seed;
    return xor128seed = (xor128seed ^ (xor128seed >> 19)) ^ (t ^ (t >> 8));
}

void LG_init(LifeGame *lg, uint64_t x, uint64_t y)
{
    lg->Generation = 0;
    lg->X = x + 2;
    lg->Y = y + 2;
    
    lg->Board = (Cell**)calloc(lg->X, sizeof(Cell*));
    
    for (uint64_t _x = 0; _x < lg->X; _x++) {
        lg->Board[_x] = (Cell*)calloc(lg->Y, sizeof(Cell));
    }
    
    for (uint64_t _y = 0; _y < lg->Y; _y++) {
        for (uint64_t _x = 0; _x < lg->X; _x++) {
            if (_x == 0 || _x == lg->X - 1 || _y == 0 || _y == lg->Y - 1) {
                lg->Board[_x][_y] = Wall;
            }else{
                if (random(0, 1) == 1) {
                    lg->Board[_x][_y] = Alive;
                }else{
                    lg->Board[_x][_y] = Dead;
                }
            }
        }
    }
}

void LG_free(LifeGame *lg)
{
    for (uint64_t _x = 0; _x < lg->X; _x++) {
        free(lg->Board[_x]);
    }
    
    free(lg->Board);
}

void LG_dump(const LifeGame *lg)
{
    printf("%lld Generation(s)\n", lg->Generation);
    
    for (uint64_t y = 0; y < lg->Y; y++) {
        for (uint64_t x = 0; x < lg->X; x++) {
            if (lg->Board[x][y] == Dead) {
                printf("  ");
            }else if (lg->Board[x][y] == Alive) {
                printf("* ");
            }else if (lg->Board[x][y] == Wall) {
                printf("# ");
            }
        }
        
        printf("\n");
    }
    
    printf("\n");
}

void LG_evo(LifeGame *lg)
{
    const Rectangular vector[] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    
    lg->Generation++;
    
    for (uint64_t y = 0; y < lg->Y; y++) {
        for (uint64_t x = 0; x < lg->X; x++) {
            if (lg->Board[x][y] == Dead) {
                int cells = 0;
                
                for (int i = 0; i < 8; i++) {
                    if (x + vector[i].x < lg->X && y + vector[i].y < lg->Y && lg->Board[x + vector[i].x][y + vector[i].y] == Alive)
                    {
                        cells++;
                    }else if (i == 5 && cells == 0) {
                        break;
                    }
                }
                
                if (cells == 3) {
                    lg->Board[x][y] = Alive_Next;
                }
            }
        }
    }
    
    for (uint64_t y = 0; y < lg->Y; y++) {
        for (uint64_t x = 0; x < lg->X; x++) {
            if (lg->Board[x][y] == Alive) {
                int cells = 0;
                
                for (int i = 0; i < 8; i++) {
                    if (x + vector[i].x < lg->X && y + vector[i].y < lg->Y && (lg->Board[x + vector[i].x][y + vector[i].y] == Alive || lg->Board[x + vector[i].x][y + vector[i].y] == Dead_Next))
                    {
                        cells++;
                    }
                }
                
                if (cells == 2 || cells == 3) {
                    //Alive
                }else{
                    lg->Board[x][y] = Dead_Next;
                }
            }
        }
    }
    
    for (uint64_t y = 0; y < lg->Y; y++) {
        for (uint64_t x = 0; x < lg->X; x++) {
            if (lg->Board[x][y] == Alive_Next) {
                lg->Board[x][y] = Alive;
            }else if(lg->Board[x][y] == Dead_Next) {
                lg->Board[x][y] = Dead;
            }
        }
    }
}