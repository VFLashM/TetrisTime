#include "pebble.h"
#include "tetrimino.h"
  
#define DIGIT_MAX_TETRIMINOS 16
#define DIGIT_WIDTH 6
#define DIGIT_HEIGHT 10
#define DIGIT_COUNT 11

#define USE_RAW_DIGITS 0
  
typedef struct {
    char letter;
    int rotation;
    int x;
    int y;
} TetriminoPos;
  
typedef struct {
    int size;
    TetriminoPos tetriminos[DIGIT_MAX_TETRIMINOS];
} DigitDef;

typedef char RawDigit[DIGIT_HEIGHT][DIGIT_WIDTH];

static void log_raw_digit(uint8_t log_level, RawDigit* raw) {
    for (int i = 0; i < DIGIT_HEIGHT; ++i) {
        APP_LOG(log_level, "Row %2d: '%.*s'", i, DIGIT_WIDTH, (*raw)[i]);
    }
}

static void reorder_digit(DigitDef* dst, const DigitDef* src) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Reordering digit with %d tetriminos", src->size);
    dst->size = 0;
    
    RawDigit buffer;
    memset(&buffer, ' ', sizeof(buffer));  

    char tetriminos_processed[DIGIT_MAX_TETRIMINOS];
    memset(&tetriminos_processed, 0, sizeof(tetriminos_processed));

    // fill raw buffer
    for (int t = 0; t < src->size; ++t) {
        const TetriminoDef* td = get_tetrimino_def(src->tetriminos[t].letter);
        const TetriminoMask* tm = &td->rotations[src->tetriminos[t].rotation];
        for (int mask_y = 0; mask_y < td->size; ++mask_y) {
            for (int mask_x = 0; mask_x < td->size; ++mask_x) {
                if ((*tm)[mask_y][mask_x]) {
                    const int x = mask_x + src->tetriminos[t].x;
                    const int y = mask_y + src->tetriminos[t].y;
                    buffer[y][x] = '0' + t;
                }
            }
        }
    }
    
    // erase raw buffer
    while (dst->size < src->size) {
        int tetrimino_fits = 0;
        //APP_LOG(APP_LOG_LEVEL_INFO, "Fitting dest tetrimino %d / %d", dst->size, src->size);
        
        const int offset = rand() % src->size;
        for (int i = 0; i < src->size; ++i) {
            const int t = (i + offset) % src->size;
            
            if (tetriminos_processed[t]) {
                //APP_LOG(APP_LOG_LEVEL_INFO, "Skipping source tetrimino %d", t);
                continue;
            }

            const TetriminoDef* td = get_tetrimino_def(src->tetriminos[t].letter);
            const TetriminoMask* tm = &td->rotations[src->tetriminos[t].rotation];

            //APP_LOG(APP_LOG_LEVEL_INFO, "Trying to fit source tetrimino %d", t);
            
            tetrimino_fits = 1;
            for (int mask_y = 0; mask_y < td->size;  ++mask_y) {
                for (int mask_x = 0; mask_x < td->size; ++mask_x) {
                    if ((*tm)[mask_y][mask_x]) {
                        for (int y = src->tetriminos[t].y + mask_y + 1; y < DIGIT_HEIGHT; ++y) {
                            const int x = src->tetriminos[t].x + mask_x;
                            const char buffer_val = buffer[y][x];
                            if (buffer_val != ('0' + t) && buffer_val != ' ') {
                                //APP_LOG(APP_LOG_LEVEL_INFO, "Source tetrimino %d failed test at %d:%d", t, x, y);
                                tetrimino_fits = 0;
                                break;
                            }
                        }
                    }
                }
                if (!tetrimino_fits) {
                    break;
                }
            }

            if (tetrimino_fits) {
                //APP_LOG(APP_LOG_LEVEL_INFO, "Source tetrimino fit %d", t);
                dst->tetriminos[dst->size] = src->tetriminos[t];
                dst->size += 1;
                tetriminos_processed[t] = 1;
                for (int mask_y = 0; mask_y < td->size; ++mask_y) {
                    for (int mask_x = 0; mask_x < td->size; ++mask_x) {
                        if ((*tm)[mask_y][mask_x]) {
                            const int x = mask_x + src->tetriminos[t].x;
                            const int y = mask_y + src->tetriminos[t].y;
                            buffer[y][x] = ' ';
                        }
                    }
                }
                break;
            }
        }

        if (!tetrimino_fits) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Can't reorder digit tetriminos, got %d out of %d:", dst->size, src->size);
            log_raw_digit(APP_LOG_LEVEL_ERROR, &buffer);
            return;
        }
    }
}

#if USE_RAW_DIGITS == 1
  
static RawDigit s_raw_digits[DIGIT_COUNT] = {
    {"tzzjjj",
     "ttzzsj",
     "tT  ss",
     "TT  ts",
     "sT  tt",
     "ss  tz",
     "is  zz",
     "it  zj",
     "ittssj",
     "itssjj"},
    
    {"    LL",
     "    lL",
     "  lllL",
     "  iiii",
     "    oo",
     "    oo",
     "    LL",
     "    lL",
     "    lL",
     "    ll"},
    
    {"llljjj",
     "liiiij",
     "    oo",
     "    oo",
     "llljjj",
     "liiiij",
     "oo    ",
     "oo    ",
     "jiiiil",
     "jjjlll"},
    
    {"llljjj",
     "liiiij",
     "    LL",
     "    lL",
     "  lllL",
     "  IIII",
     "    Ji",
     "    Ji",
     "oojJJi",
     "oojjji"},

    {"OO  LL",
     "OO  lL",
     "oo  lL",
     "oo  ll",
     "llljjj",
     "liiiij",
     "    JJ",
     "    Jj",
     "    Jj",
     "    jj"},

    {"llljjj",
     "liiiij",
     "oo    ",
     "oo    ",
     "llljjj",
     "liiiij",
     "    oo",
     "    oo",
     "jiiiil",
     "jjjlll"},

    {"llljjj",
     "liiiij",
     "oo    ",
     "oo    ",
     "TTTsst",
     "tTsstt",
     "tt  st",
     "tJ  ss",
     "jJJJts",
     "jjjttt"},

    {"llljjj",
     "liiiij",
     "    LL",
     "    lL",
     "    lL",
     "    ll",
     "    JJ",
     "    Jj",
     "    Jj",
     "    jj"},

    {"stttss",
     "sstsst",
     "Ts  tt",
     "TT  lt",
     "Tjlllz",
     "tjjjzz",
     "tt  zt",
     "tz  tt",
     "zztzzt",
     "ztttzz"},

    {"tzzjjj",
     "ttzztj",
     "tz  tt",
     "zz  tl",
     "zsslll",
     "ssiiii",
     "    LL",
     "    lL",
     "jJJJlL",
     "jjjJll"},

    {"      ",
     "      ",
     "  oo  ",
     "  oo  ",
     "      ",
     "      ",
     "  oo  ",
     "  oo  ",
     "      ",
     "      "},
};

static int match_mask(RawDigit* raw, const TetriminoMask* mask, int mask_size, int pos_x, int pos_y, char letter, int clear) {
    for (int mask_y = 0; mask_y < mask_size; ++mask_y) {
        for (int mask_x = 0; mask_x < mask_size; ++mask_x) {
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

    for (int t = 0; t < TETRIMINO_COUNT; ++t) {
        const char def_letter = s_tetrimino_defs[t].letter;
        for (int r = 0; r < 4; ++r) {
            const TetriminoDef* td = &s_tetrimino_defs[t];
            const TetriminoMask* tm = &td->rotations[r];
      
            // find first occupied mask position
            int mask_start_x = 0;
            int mask_start_y = 0;
            for (; mask_start_y < td->size; ++mask_start_y) {
                for (mask_start_x = 0; mask_start_x < td->size; ++mask_start_x) {
                    if ((*tm)[mask_start_y][mask_start_x]) {
                        break;
                    }
                }
                if (mask_start_x < td->size) {
                    break;
                }
            }
            //APP_LOG(APP_LOG_LEVEL_INFO, "Processing %c:%d tetrimino with first block at %d:%d", def_letter, r, mask_start_x, mask_start_y);
      
            for (int pos_y = -td->size; pos_y < DIGIT_HEIGHT + td->size; ++pos_y) {
                for (int pos_x = -td->size; pos_x < DIGIT_WIDTH + td->size; ++pos_x) {
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
          
                    if (match_mask(raw, tm, td->size, pos_x, pos_y, start_letter, 0)) {
                        //APP_LOG(APP_LOG_LEVEL_INFO, "Matched %c:%d tetrimino at %d:%d", def_letter, r, pos_x, pos_y);
                        match_mask(raw, tm, td->size, pos_x, pos_y, start_letter, 1);            
                        if (def->size < DIGIT_MAX_TETRIMINOS) {
                            def->tetriminos[def->size].letter = def_letter;
                            def->tetriminos[def->size].rotation = r;
                            def->tetriminos[def->size].x = pos_x;
                            def->tetriminos[def->size].y = pos_y;
                            def->size += 1;
                        }
                    }
                }
            }
        }    
    }
  
    for (int y = 0; y < DIGIT_HEIGHT; ++y) {
        for (int x = 0; x < DIGIT_WIDTH; ++x) {
            if ((*raw)[y][x] != ' ') {
                APP_LOG(APP_LOG_LEVEL_ERROR, "Can't parse raw digit, matched %d tetriminos:", def->size);
                log_raw_digit(APP_LOG_LEVEL_ERROR, raw);
                return 0;
            }      
        }
    }

    APP_LOG(APP_LOG_LEVEL_INFO, "Parsed raw digit with %d tetriminos", def->size);
    return def->size;
}

static void format_digit_def_struct(const DigitDef* def) {
    APP_LOG(APP_LOG_LEVEL_INFO, "{ %d,", def->size);
    APP_LOG(APP_LOG_LEVEL_INFO, "  {");
    for (int i = 0; i < def->size; ++i) {
        const TetriminoPos* tp = &def->tetriminos[i];
        APP_LOG(APP_LOG_LEVEL_INFO, "    {'%c', %d, %d, %d},", tp->letter, tp->rotation, tp->x, tp->y);
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "  }");
    APP_LOG(APP_LOG_LEVEL_INFO, "},");
}

static DigitDef s_digits[DIGIT_COUNT];

#else

static DigitDef s_digits[DIGIT_COUNT] = {
    { 12,
      {
          {'i', 1, -2, 6},
          {'j', 3, 4, 7},
          {'s', 0, 2, 8},
          {'t', 1, 0, 7},
          {'s', 1, -1, 4},
          {'t', 3, 0, 2},
          {'t', 1, -1, 0},
          {'z', 0, 1, 0},
          {'z', 1, 3, 5},
          {'t', 1, 3, 3},
          {'s', 1, 3, 1},
          {'j', 2, 3, -1},
      }
    },
    { 6,
      {
          {'l', 1, 3, 7},
          {'l', 3, 4, 6},
          {'o', 0, 4, 4},
          {'i', 0, 2, 2},
          {'l', 0, 2, 1},
          {'l', 3, 4, 0},
      }
    },
    { 11,
      {
          {'j', 0, 0, 8},
          {'l', 0, 3, 8},
          {'i', 0, 1, 7},
          {'o', 0, 0, 6},
          {'i', 0, 1, 4},
          {'j', 2, 3, 3},
          {'l', 2, 0, 3},
          {'o', 0, 4, 2},
          {'i', 0, 1, 0},
          {'j', 2, 3, -1},
          {'l', 2, 0, -1},
      }
    },
    { 10,
      {
          {'o', 0, 0, 8},
          {'i', 1, 3, 6},
          {'j', 0, 2, 8},
          {'j', 3, 3, 6},
          {'i', 0, 2, 4},
          {'l', 0, 2, 3},
          {'l', 3, 4, 2},
          {'i', 0, 1, 0},
          {'j', 2, 3, -1},
          {'l', 2, 0, -1},
      }
    },
    { 9,
      {
          {'j', 3, 4, 7},
          {'j', 1, 3, 6},
          {'i', 0, 1, 4},
          {'l', 2, 0, 3},
          {'o', 0, 0, 2},
          {'j', 2, 3, 3},
          {'l', 1, 3, 1},
          {'l', 3, 4, 0},
          {'o', 0, 0, 0},
      }
    },
    { 11,
      {
          {'j', 0, 0, 8},
          {'l', 0, 3, 8},
          {'i', 0, 1, 7},
          {'o', 0, 4, 6},
          {'i', 0, 1, 4},
          {'j', 2, 3, 3},
          {'l', 2, 0, 3},
          {'o', 0, 0, 2},
          {'i', 0, 1, 0},
          {'j', 2, 3, -1},
          {'l', 2, 0, -1},
      }
    },
    { 12,
      {
          {'t', 0, 3, 8},
          {'j', 0, 0, 8},
          {'s', 1, 3, 6},
          {'t', 3, 4, 4},
          {'j', 0, 1, 7},
          {'s', 0, 2, 4},
          {'t', 1, -1, 5},
          {'t', 2, 0, 3},
          {'o', 0, 0, 2},
          {'i', 0, 1, 0},
          {'j', 2, 3, -1},
          {'l', 2, 0, -1},
      }
    },
    { 7,
      {
          {'j', 3, 4, 7},
          {'j', 1, 3, 6},
          {'l', 1, 3, 3},
          {'l', 3, 4, 2},
          {'i', 0, 1, 0},
          {'l', 2, 0, -1},
          {'j', 2, 3, -1},
      }
    },
    { 13,
      {
          {'t', 0, 1, 8},
          {'z', 0, 3, 8},
          {'t', 3, 4, 6},
          {'z', 1, 3, 4},
          {'z', 1, -1, 7},
          {'t', 1, -1, 5},
          {'j', 0, 1, 4},
          {'t', 1, -1, 2},
          {'l', 0, 2, 3},
          {'s', 1, -1, 0},
          {'t', 3, 4, 1},
          {'s', 0, 3, 0},
          {'t', 2, 1, -1},
      }
    },
    { 12,
      {
          {'l', 1, 3, 7},
          {'l', 3, 4, 6},
          {'j', 0, 0, 8},
          {'j', 2, 1, 7},
          {'i', 0, 2, 4},
          {'l', 0, 3, 3},
          {'s', 0, 0, 4},
          {'z', 1, -1, 2},
          {'t', 1, -1, 0},
          {'z', 0, 1, 0},
          {'t', 1, 3, 1},
          {'j', 2, 3, -1},
      }
    },
    { 2,
      {
          {'o', 0, 2, 6},
          {'o', 0, 2, 2},
      }
    },
};

#endif

