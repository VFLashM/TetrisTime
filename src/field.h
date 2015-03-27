#define FIELD_WIDTH 36
#define FIELD_HEIGHT 42
#define FIELD_CELL_SIZE 3
#define FIELD_CELL_SPACING 1
#define FIELD_OFFSET_X 0
#define FIELD_OFFSET_Y 1

typedef enum {
    COLOR_TRANSPARENT,
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_MAX
} PaletteColor;

GColor s_palette[COLOR_MAX] = {
    GColorClear,
    GColorBlack,
    GColorWhite,
};


static int s_field_inited;
static PaletteColor s_field_bg_color;
static PaletteColor s_last_field[FIELD_HEIGHT][FIELD_WIDTH];
static PaletteColor s_next_field[FIELD_HEIGHT][FIELD_WIDTH];

static void field_reset(PaletteColor background) {
    s_field_bg_color = background;
    for (int j = 0; j < FIELD_HEIGHT; ++j) {
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            s_last_field[j][i] = background;
            s_next_field[j][i] = background;
        }
    }
    s_field_inited = 0;
}

static void field_draw(int x, int y, PaletteColor color) {
    if (x >= 0 && y >= 0 && x < FIELD_WIDTH && y < FIELD_HEIGHT) {
        s_next_field[y][x] = color;
    }
}

static void log_field_state() {
    static char outbuf[2048];
    char* out = outbuf;
    //time_t seconds;
    //uint16_t mseconds = time_ms(&seconds, 0);
    int color = s_last_field[0][0];
    int color_count = 0;
    const int max_color_count = '~' - '"';

    // fill
    *out++ = '!';
    //*out++ = '"' + (seconds % 100);
    //*out++ = '"' + ((mseconds / 10) % 100);
    *out++ = '0' + color;
    for (int j = 0; j < FIELD_HEIGHT; ++j) {
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            if (s_last_field[j][i] == color && color_count < max_color_count) {
                color_count += 1;
            } else {
                *out++ = '"' + color_count;
                if ((color_count == max_color_count) != (s_last_field[j][i] == color)) {
                    *out++ = '"';
                }
                color = s_last_field[j][i];
                color_count = 1;
            }
        }
    }
    if (color_count) {
        *out++ = '"' + color_count;
    }
    *out++ = '!';

    // print
    *out = 0;
    out = outbuf;
    const int line_len = 60;
    while ((int)strlen(out) > line_len) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "%.*s", line_len, out);
        out += line_len;
    }
    APP_LOG(APP_LOG_LEVEL_ERROR, "%s", out);
}

static void field_flush(Layer* layer, GContext* ctx) {
    GRect rect;
    
    if (!s_field_inited) {
        rect = layer_get_bounds(layer);
        graphics_context_set_fill_color(ctx, s_palette[s_field_bg_color]);
        graphics_fill_rect(ctx, rect, 0, GCornerNone);
        s_field_inited = 1;
    }
    
    rect.size.h = FIELD_CELL_SIZE;
    rect.size.w = FIELD_CELL_SIZE;
    
    for (int j = 0; j < FIELD_HEIGHT; ++j) {
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            const PaletteColor next = s_next_field[j][i];
            if (next != s_last_field[j][i]) {
                s_last_field[j][i] = next;
                
                graphics_context_set_fill_color(ctx, s_palette[next]);
                rect.origin.x = FIELD_OFFSET_X + i * (FIELD_CELL_SIZE + FIELD_CELL_SPACING);
                rect.origin.y = FIELD_OFFSET_Y + j * (FIELD_CELL_SIZE + FIELD_CELL_SPACING);
                graphics_fill_rect(ctx, rect, 0, GCornerNone);
            }
            s_next_field[j][i] = s_field_bg_color;
        }
    }

    //log_field_state();
}
