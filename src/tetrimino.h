#define TETRIMINO_COUNT 7  
#define TETRIMINO_MASK_SIZE 4

typedef uint8_t TetriminoMask[TETRIMINO_MASK_SIZE][TETRIMINO_MASK_SIZE];

#define COLOR_TO_BYTE(basalt, aplite) PBL_IF_COLOR_ELSE(basalt ## ARGB8, aplite ## ARGB8)

typedef struct {
    char letter;
    uint8_t size;
    uint8_t unique_shapes;
    uint8_t color;
    TetriminoMask rotations[4];
} TetriminoDef;

static const TetriminoDef s_tetrimino_defs[TETRIMINO_COUNT] = {
    { 'i', 4, 2, COLOR_TO_BYTE(GColorCyan, GColorWhite),
        {
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
    { 'j', 3, 4, COLOR_TO_BYTE(GColorBlueMoon, GColorWhite),
        {
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
    { 'l', 3, 4, COLOR_TO_BYTE(GColorOrange, GColorWhite),
        {
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
    { 'o', 2, 1, COLOR_TO_BYTE(GColorLimerick, GColorWhite),
        {
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
    { 's', 3, 2, COLOR_TO_BYTE(GColorGreen, GColorWhite),
        {
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
    { 't', 3, 4, COLOR_TO_BYTE(GColorPurple, GColorWhite),
        {
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
    { 'z', 3, 2, COLOR_TO_BYTE(GColorRed, GColorWhite),
        {
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
