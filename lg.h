#ifndef maze_lg_h
#define maze_lg_h

typedef enum tagCell
{
    Dead, Alive, Wall, Alive_Next, Dead_Next
} Cell;

typedef struct tagLifeGame
{
    uint64_t Generation;
    uint64_t X;
    uint64_t Y;
    Cell **Board;
} LifeGame;

void LG_init(LifeGame *lg, uint64_t x, uint64_t y);
void LG_free(LifeGame *lg);
void LG_dump(const LifeGame *lg);
void LG_evo(LifeGame *lg);

#endif
