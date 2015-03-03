#define TETRIMINO_COUNT 7  
#define TETRIMINO_MASK_SIZE 4

typedef int TetriminoMask[TETRIMINO_MASK_SIZE][TETRIMINO_MASK_SIZE];

typedef struct {
    char letter;
    int size;
    TetriminoMask rotations[4];
} TetriminoDef;

static const TetriminoDef s_tetrimino_defs[TETRIMINO_COUNT] = {
    { 'i', 4, {
            {{0,0,0,0},
             {1,1,1,1},
             {0,0,0,0},
             {0,0,0,0}},
            {{0,0,1,0},
             {0,0,1,0},
             {0,0,1,0},
             {0,0,1,0}},
            {{0,0,0,0},
             {0,0,0,0},
             {1,1,1,1},
             {0,0,0,0}},
            {{0,1,0,0},
             {0,1,0,0},
             {0,1,0,0},
             {0,1,0,0}}
        }},
    { 'j', 3, {
            {{1,0,0,0},
             {1,1,1,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{0,1,1,0},
             {0,1,0,0},
             {0,1,0,0},
             {0,0,0,0}},
            {{0,0,0,0},
             {1,1,1,0},
             {0,0,1,0},
             {0,0,0,0}},
            {{0,1,0,0},
             {0,1,0,0},
             {1,1,0,0},
             {0,0,0,0}}
        }},
    { 'l', 3, {
            {{0,0,1,0},
             {1,1,1,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{0,1,0,0},
             {0,1,0,0},
             {0,1,1,0},
             {0,0,0,0}},
            {{0,0,0,0},
             {1,1,1,0},
             {1,0,0,0},
             {0,0,0,0}},
            {{1,1,0,0},
             {0,1,0,0},
             {0,1,0,0},
             {0,0,0,0}}
        }},
    { 'o', 2, {
            {{1,1,0,0},
             {1,1,0,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{1,1,0,0},
             {1,1,0,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{1,1,0,0},
             {1,1,0,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{1,1,0,0},
             {1,1,0,0},
             {0,0,0,0},
             {0,0,0,0}}
        }},
    { 's', 3, {
            {{0,1,1,0},
             {1,1,0,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{0,1,0,0},
             {0,1,1,0},
             {0,0,1,0},
             {0,0,0,0}},
            {{0,0,0,0},
             {0,1,1,0},
             {1,1,0,0},
             {0,0,0,0}},
            {{1,0,0,0},
             {1,1,0,0},
             {0,1,0,0},
             {0,0,0,0}}
        }},
    { 't', 3, {
            {{0,1,0,0},
             {1,1,1,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{0,1,0,0},
             {0,1,1,0},
             {0,1,0,0},
             {0,0,0,0}},
            {{0,0,0,0},
             {1,1,1,0},
             {0,1,0,0},
             {0,0,0,0}},
            {{0,1,0,0},
             {1,1,0,0},
             {0,1,0,0},
             {0,0,0,0}}
        }},
    { 'z', 3, {
            {{1,1,0,0},
             {0,1,1,0},
             {0,0,0,0},
             {0,0,0,0}},
            {{0,0,1,0},
             {0,1,1,0},
             {0,1,0,0},
             {0,0,0,0}},
            {{0,0,0,0},
             {1,1,0,0},
             {0,1,1,0},
             {0,0,0,0}},
            {{0,1,0,0},
             {1,1,0,0},
             {1,0,0,0},
             {0,0,0,0}}
        }},
};

static const TetriminoDef* get_tetrimino_def(char letter) {
    switch (letter) {
    case 'i': return &s_tetrimino_defs[0];
    case 'j': return &s_tetrimino_defs[1];
    case 'l': return &s_tetrimino_defs[2];
    case 'o': return &s_tetrimino_defs[3];
    case 's': return &s_tetrimino_defs[4];
    case 't': return &s_tetrimino_defs[5];
    case 'z': return &s_tetrimino_defs[6];
    default: return NULL;
    }
    /*
    for (int i = 0; i < TETRIMINO_COUNT; ++i) {
        if (s_tetrimino_defs[i].letter == letter) {
            return &s_tetrimino_defs[i];
        }
    }
    return NULL;
    */
}
