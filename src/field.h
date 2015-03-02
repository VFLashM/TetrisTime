#define FIELD_WIDTH 36
#define FIELD_HEIGHT 24
#define FIELD_CELL_SIZE 3
#define FIELD_CELL_SPACING 1
#define FIELD_OFFSET_X 0
#define FIELD_OFFSET_Y 1


static GColor s_last_field[FIELD_HEIGHT][FIELD_WIDTH];
static GColor s_next_field[FIELD_HEIGHT][FIELD_WIDTH];

static void field_init(GColor background) {
    for (int j = 0; j < FIELD_HEIGHT; ++j) {
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            s_last_field[j][i] = GColorClear;
            s_next_field[j][i] = background;
        }
    }
}

static void field_draw(int x, int y, GColor color) {
    if (x >= 0 && y >= 0 && x < FIELD_WIDTH && y < FIELD_HEIGHT) {
        s_next_field[y][x] = color;
    }
}

static void field_flush(Layer* layer, GContext* ctx, GColor background) {
    GRect rect;
    rect.size.h = FIELD_CELL_SIZE;
    rect.size.w = FIELD_CELL_SIZE;
    
    for (int j = 0; j < FIELD_HEIGHT; ++j) {
        for (int i = 0; i < FIELD_WIDTH; ++i) {
            GColor next = s_next_field[j][i];
            if (memcmp(&s_last_field[j][i], &next, sizeof(GColor))) {
                s_last_field[j][i] = next;
                
                graphics_context_set_fill_color(ctx, next);
                rect.origin.x = FIELD_OFFSET_X + i * (FIELD_CELL_SIZE + FIELD_CELL_SPACING);
                rect.origin.y = FIELD_OFFSET_Y + j * (FIELD_CELL_SIZE + FIELD_CELL_SPACING);
                graphics_fill_rect(ctx, rect, 0, GCornerNone);
            }
            s_next_field[j][i] = background;
        }
    }
}
