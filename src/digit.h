#include "pebble.h"
#include "tetrimino.h"
  
#define DIGIT_MAX_TETRIMINOS 32
#define DIGIT_WIDTH 6
#define DIGIT_HEIGHT 10
#define USE_RAW_DIGITS 1
  
typedef struct {
    char letter;
    int rotation;
    int x;
    int y;
} TetriminoPos;
  
typedef struct {
    TetriminoPos tetriminos[DIGIT_MAX_TETRIMINOS];
} DigitDef;

static DigitDef s_digits[10];

#if USE_RAW_DIGITS == 1
  
typedef char RawDigit[DIGIT_HEIGHT][DIGIT_WIDTH];

static RawDigit s_raw_digits[1] = {
    {"tzzjjj",
     "ttzzsj",
     "tT  ss",
     "TT  ts",
     "sT  tt",
     "ss  tz",
     "is  zz",
     "it  zj",
     "ittssj",
     "itssjj"}
};

static int tetrimino_pos_comparator(const void* v1, const void* v2) {
    const TetriminoPos* p1 = (const TetriminoPos*)(v1);
    const TetriminoPos* p2 = (const TetriminoPos*)(v2);
    if (p1->y > p2->y) {
        return -1;
    } else if (p1->y < p2->y) {
        return 1;
    } else {
        return 0;
    }
}

static int match_mask(RawDigit* raw, const TetriminoMask* mask, int pos_x, int pos_y, char letter, int clear) {
    for (int mask_y = 0; mask_y < TETRIMINO_MASK_SIZE; ++mask_y) {
        for (int mask_x = 0; mask_x < TETRIMINO_MASK_SIZE; ++mask_x) {
            if (!(*mask)[mask_y][mask_x]) {
                continue;
            }
            int x = pos_x + mask_x;
            int y = pos_y + mask_y;
            if (x < 0 || y < 0 || x >= DIGIT_WIDTH || y >= DIGIT_HEIGHT) {
                return 0;
            }
            if ((*raw)[y][x] != letter) {
                return 0;
            }
            if (clear) {
                (*raw)[y][x] = ' ';
            }
        }
    }
    return 1;
}

static int parse_raw_digit(DigitDef* def, RawDigit* raw) {
    memset(def, 0, sizeof(*def));
  
    int def_size = 0;
  
    for (int t = 0; t < TETRIMINO_COUNT; ++t) {
        const char def_letter = s_tetrimino_defs[t].letter;
        for (int r = 0; r < 4; ++r) {
            const TetriminoMask* mask = &s_tetrimino_defs[t].rotations[r];
      
            // find first occupied mask position
            int mask_start_x = 0;
            int mask_start_y = 0;
            for (; mask_start_y < TETRIMINO_MASK_SIZE; ++mask_start_y) {
                for (mask_start_x = 0; mask_start_x < TETRIMINO_MASK_SIZE; ++mask_start_x) {
                    if ((*mask)[mask_start_y][mask_start_x]) {
                        break;
                    }
                }
                if (mask_start_x < TETRIMINO_MASK_SIZE) {
                    break;
                }
            }
            //APP_LOG(APP_LOG_LEVEL_INFO, "Processing %c:%d tetrimino with first block at %d:%d", def_letter, r, mask_start_x, mask_start_y);
      
            for (int pos_y = -TETRIMINO_MASK_SIZE; pos_y < DIGIT_HEIGHT + TETRIMINO_MASK_SIZE; ++pos_y) {
                for (int pos_x = -TETRIMINO_MASK_SIZE; pos_x < DIGIT_WIDTH + TETRIMINO_MASK_SIZE; ++pos_x) {
                    int x = pos_x + mask_start_x;
                    int y = pos_y + mask_start_y;
          
                    if (x < 0 || y < 0 || x >= DIGIT_WIDTH || y >= DIGIT_HEIGHT) {
                        continue;
                    }
          
                    const char start_letter = (*raw)[y][x];
                    if (def_letter != start_letter && def_letter != (start_letter - 'A' + 'a')) {
                        continue;
                    }
          
                    //APP_LOG(APP_LOG_LEVEL_INFO, "Trying to match %c:%d tetrimino at %d:%d", def_letter, r, pos_x, pos_y);
          
                    if (match_mask(raw, mask, pos_x, pos_y, start_letter, 0)) {
                        //APP_LOG(APP_LOG_LEVEL_INFO, "Matched %c:%d tetrimino at %d:%d", def_letter, r, pos_x, pos_y);
                        match_mask(raw, mask, pos_x, pos_y, start_letter, 1);            
                        if (def_size < DIGIT_MAX_TETRIMINOS) {
                            def->tetriminos[def_size].letter = def_letter;
                            def->tetriminos[def_size].rotation = r;
                            def->tetriminos[def_size].x = pos_x;
                            def->tetriminos[def_size].y = pos_y;
                            def_size += 1;
                        }
                    }
                }
            }
        }    
    }
  
    for (int y = 0; y < DIGIT_HEIGHT; ++y) {
        for (int x = 0; x < DIGIT_WIDTH; ++x) {
            if ((*raw)[y][x] != ' ') {
                APP_LOG(APP_LOG_LEVEL_ERROR, "Can't parse raw digit, matched %d tetriminos:", def_size);
                for (int i = 0; i < DIGIT_HEIGHT; ++i) {
                    APP_LOG(APP_LOG_LEVEL_ERROR, "Row %2d: %.*s", i, DIGIT_WIDTH, (*raw)[i]);
                }
                return 0;
            }      
        }
    }

    APP_LOG(APP_LOG_LEVEL_INFO, "Parsed digit with %d tetriminos", def_size);
    qsort(&def->tetriminos, def_size, sizeof(TetriminoPos), tetrimino_pos_comparator);
    return def_size;
}
#endif

