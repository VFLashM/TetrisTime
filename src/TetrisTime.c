#include <pebble.h>
#include "digit.h"
#include "field.h"
#include "settings.h"
  
#define ANIMATION_TIMEOUT_MS 100
#define DIGIT_COUNT 4
#define SECOND_DOT_COUNT 2

typedef struct {
    int offset_x;
    int offset_y;
    
    char active;

    int target_value;
    int next_value;
    DigitDef target;
    DigitDef current;
} DigitState;

static GColor s_bg_color;
static GColor s_fg_color;

static int s_animating;
static Window *s_window;
static Layer *s_layer;

static DigitState s_states[DIGIT_COUNT];
static int s_show_second_dot = 1;
static TetriminoPos s_second_dot_pos[SECOND_DOT_COUNT];

static void state_step(DigitState* state) {
    if (!state->active) {
        if (state->next_value != state->target_value) {
            APP_LOG(APP_LOG_LEVEL_INFO, "Digit target changed to %d", state->next_value);
            state->target_value = state->next_value;
            reorder_digit(&state->target, &s_digits[state->target_value]);
            state->current.size = 0;
            state->active = 1;
        } else {
            return;
        }
    }
    
    const int start_y = -TETRIMINO_MASK_SIZE * 2;
    
    int last_y = TETRIMINO_MASK_SIZE;
    for (int i = 0; i < state->current.size; ++i) {
        TetriminoPos* current_pos = &state->current.tetriminos[i];
        const TetriminoPos* target_pos = &state->target.tetriminos[i];
        if (current_pos->rotation != target_pos->rotation) {
            current_pos->rotation = (current_pos->rotation + 1) % 4;
        }
        if (current_pos->y < target_pos->y) {
            current_pos->y += 1;
        } 
        if (current_pos->x < target_pos->x) {
            current_pos->x += 1;
        } else if (current_pos->x > target_pos->x) {
            current_pos->x -= 1;
        }
        last_y = current_pos->y;
    }
    
    if (last_y >= (start_y + TETRIMINO_MASK_SIZE)) {
        if (state->current.size < state->target.size) {
            const char target_letter = state->target.tetriminos[state->current.size].letter;
            const TetriminoDef* td = get_tetrimino_def(target_letter); 
            TetriminoPos* current_pos = &state->current.tetriminos[state->current.size];
            current_pos->letter = target_letter;
            current_pos->x = rand() % (DIGIT_WIDTH - td->size + 1);
            current_pos->y = start_y;
            current_pos->rotation = rand() % 4;
            state->current.size += 1;
        }
    }

    if (state->current.size == state->target.size) {
        if (state->current.size == 0 ||
            memcmp(&state->current.tetriminos[state->current.size-1],
                   &state->target.tetriminos[state->target.size-1],
                   sizeof(TetriminoPos)) == 0)
        {
            state->active = 0;
        }
    }
}

static void draw_tetrimino(const TetriminoPos* tp, int offset_x, int offset_y) {
    const TetriminoDef* td = get_tetrimino_def(tp->letter); 
    const TetriminoMask* tm = &td->rotations[tp->rotation];

    for (int mask_y = 0; mask_y < TETRIMINO_MASK_SIZE; ++mask_y) {
        for (int mask_x = 0; mask_x < TETRIMINO_MASK_SIZE; ++mask_x) {
            if ((*tm)[mask_y][mask_x]) {
                const int x = tp->x + mask_x + offset_x;
                const int y = tp->y + mask_y + offset_y;
                field_draw(x, y, s_fg_color);
            }
        }
    }
}

static void draw_digit_def(const DigitDef* def, int offset_x, int offset_y) {
    for (int i = 0; i < def->size; ++i) {
        draw_tetrimino(&def->tetriminos[i], offset_x, offset_y);
    }
}

static void layer_draw(Layer* layer, GContext* ctx) {
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        draw_digit_def(&s_states[i].current, s_states[i].offset_x, s_states[i].offset_y);
    }
    if (s_show_second_dot) {
        for (int i = 0; i < SECOND_DOT_COUNT; ++i) {
            draw_tetrimino(&s_second_dot_pos[i], 0, 0);
        }
    }
    field_flush(layer, ctx, s_bg_color);
}


static void process_animation(void* data) {
    /* static int st = 0; */
    /* APP_LOG(APP_LOG_LEVEL_INFO, "Step %d", st); */
    /* st += 1; */
    
    s_animating = 1;
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        state_step(&s_states[i]);
    }
    layer_mark_dirty(s_layer);
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        if (s_states[i].active) {
            app_timer_register(ANIMATION_TIMEOUT_MS, process_animation, NULL);
            return;
        }
    }
    s_animating = 0;
}

static void main_window_load(Window *window) {
    s_layer = window_get_root_layer(window);

    field_init(s_bg_color);
    layer_set_update_proc(s_layer, layer_draw);
}

static void main_window_unload(Window *window) {
}

static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
    int digit_values[DIGIT_COUNT];
    digit_values[0] = tick_time->tm_hour / 10;
    digit_values[1] = tick_time->tm_hour % 10;
    digit_values[2] = tick_time->tm_min / 10;
    digit_values[3] = tick_time->tm_min % 10;

    int changed = 0;
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        const int value = digit_values[i];
        if (s_states[i].next_value != value) {
            s_states[i].next_value = value;
            APP_LOG(APP_LOG_LEVEL_INFO, "Digit %d scheduled to be %d", i, value);
            changed = 1;
        }
    }

    if (changed && !s_animating) {
        process_animation(NULL);
    }

    if (s_settings[ANIMATE_SECOND_DOT]) {
        s_show_second_dot = tick_time->tm_sec % 2;
        layer_mark_dirty(s_layer);
    }
}

static void on_settings_changed() {
    if (s_settings[SPARSE_DIGITS]) {
        s_states[0].offset_x = 1;
        s_states[1].offset_x = 9;
        s_states[2].offset_x = 21;
        s_states[3].offset_x = 29;
    } else {
        s_states[0].offset_x = 2;
        s_states[1].offset_x = 10;
        s_states[2].offset_x = 20;
        s_states[3].offset_x = 28;
    }

    if (s_settings[DARK_THEME]) {
        s_bg_color = GColorBlack;
        s_fg_color = GColorWhite;
    } else {
        s_bg_color = GColorWhite;
        s_fg_color = GColorBlack;
    }

    tick_timer_service_unsubscribe();
    if (s_settings[ANIMATE_SECOND_DOT]) {
        tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    } else {
        tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    }

    if (s_layer) {
        layer_mark_dirty(s_layer);
    }
    field_init(s_bg_color);
}

static void in_received_handler(DictionaryIterator* iter, void* context)
{
    APP_LOG(APP_LOG_LEVEL_INFO, "Received settings");
    settings_read(iter);
    on_settings_changed();
}
  
static void init() {
    srand(time(NULL));

    settings_load_persistent();
    app_message_register_inbox_received(in_received_handler);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    settings_send();
    
#if USE_RAW_DIGITS == 1
    for (int i = 0; i < 10; ++i) {
        if (!parse_raw_digit(&s_digits[i], &s_raw_digits[i])) {
            return;
        }
    }
#endif

    for (int i = 0; i < DIGIT_COUNT; ++i) {
        s_states[i].next_value = 10;
        s_states[i].target_value = 10;
        s_states[i].offset_y = FIELD_HEIGHT - DIGIT_HEIGHT;
    }
    for (int i = 0; i < SECOND_DOT_COUNT; ++i) {
        s_second_dot_pos[i].x = 17;
        s_second_dot_pos[i].letter = 'o';
    }
    s_second_dot_pos[0].y = FIELD_HEIGHT - DIGIT_HEIGHT + 2;
    s_second_dot_pos[1].y = FIELD_HEIGHT - DIGIT_HEIGHT + 6;
    
    
    // Create main Window element and assign to pointer
    s_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_window, (WindowHandlers) {
            .load = main_window_load,
                .unload = main_window_unload
                });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_window, true);

    on_settings_changed();
}

static void deinit() {
    // Destroy Window
    window_destroy(s_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
