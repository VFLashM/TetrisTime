#include <pebble.h>
#include "digit.h"
#include "field.h"
#include "settings.h"
#include "bitmap.h"


// real const
#define STATE_COUNT 5
#define ANIMATION_SPACING_Y (TETRIMINO_MASK_SIZE + 1)

// debug settings
#define DYNAMIC_ASSEMBLY 0

typedef struct {
    int offset_x;
    int offset_y;
    
    char falling;

    int target_value;
    int next_value;
    DigitDef target;
    DigitDef current;

    int action_height;
    int vanishing_frame;

    int restricted_spawn_width;
} DigitState;

// time state
static int s_show_second_dot = 1;
static int s_month;
static int s_day;
static int s_weekday;

// color state
static GColor s_bg_color;
static GColor s_fg_color;

// pebbele infrastructure
static int s_animating;
static Window* s_window;
static Layer* s_layer;

// digit states
static DigitState s_states[STATE_COUNT];
static int s_date_frame;

static void state_step(DigitState* state) {
    if (!state->falling) {
        if (state->next_value != state->target_value) {
            const int animation_period_frames = s_settings[CUSTOM_ANIMATION_PERIOD_VIS_FRAMES] + s_settings[CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES];
            if (state->vanishing_frame > s_settings[CUSTOM_ANIMATION_PERIOD_COUNT] * animation_period_frames) {
                //APP_LOG(APP_LOG_LEVEL_INFO, "Digit target changed to %d", state->next_value);
                state->target_value = state->next_value;
                if (DYNAMIC_ASSEMBLY) {
                    reorder_digit(&state->target, &s_digits[state->target_value]);
                } else {
                    state->target = s_digits[state->target_value];
                }
                state->current.size = 0;
                state->falling = 1;
                state->vanishing_frame = 0;
            } else {
                state->vanishing_frame += 1;
                return;
            }
        } else {
            return;
        }
    }
   
    int last_y = TETRIMINO_MASK_SIZE;
    for (int i = 0; i < state->current.size; ++i) {
        TetriminoPos* current_pos = &state->current.tetriminos[i];
        const TetriminoPos* target_pos = &state->target.tetriminos[i];

        const int height_remaining = target_pos->y - current_pos->y;
        const int moves_needed = abs(target_pos->x - current_pos->x);
        int rotations_needed = target_pos->rotation - current_pos->rotation;
        if (rotations_needed < 0) { rotations_needed += 4; }
        const int actions_needed = moves_needed + rotations_needed;

        if (state->action_height >= current_pos->y) {
            int step = height_remaining / (actions_needed + 1);
            state->action_height = current_pos->y + step;
        }

        if (current_pos->y < target_pos->y) {
            current_pos->y += 1;
        }

        if (current_pos->y >= state->action_height) {
            if (moves_needed > rotations_needed) {
                if (current_pos->x < target_pos->x) {
                    current_pos->x += 1;
                } else if (current_pos->x > target_pos->x) {
                    current_pos->x -= 1;
                }
            } else if (rotations_needed) {
                current_pos->rotation = (current_pos->rotation + 1) % 4;
            }
        }
        
     
        last_y = current_pos->y;
    }
    
    
    if (state->current.size < state->target.size) {
        const TetriminoPos* target_pos = &state->target.tetriminos[state->current.size];
        const char target_letter = target_pos->letter;
        const TetriminoDef* td = get_tetrimino_def(target_letter);

        const int start_y = -state->offset_y - td->size + 1;
        if (last_y >= (start_y + ANIMATION_SPACING_Y)) {
            TetriminoPos* current_pos = &state->current.tetriminos[state->current.size];
            current_pos->letter = target_letter;
            if (state->restricted_spawn_width) {
                const int spawn_width = 4;
                current_pos->x = rand() % (spawn_width - td->size + 1);
                current_pos->x += (DIGIT_WIDTH - spawn_width) / 2;
            } else {
                current_pos->x = rand() % (DIGIT_WIDTH - td->size + 1);
            }
            current_pos->y = start_y;
            const int rotation_unique = rand() % td->unique_shapes;
            current_pos->rotation = (target_pos->rotation - rotation_unique + 4) % 4;
            state->action_height = start_y;
            state->current.size += 1;
        }
    }

    if (state->current.size == state->target.size) {
        if (state->current.size == 0 ||
            memcmp(&state->current.tetriminos[state->current.size-1],
                   &state->target.tetriminos[state->target.size-1],
                   sizeof(TetriminoPos)) == 0)
        {
            state->falling = 0;
        }
    }
}

static void draw_weekday_line(int height, GColor color) {
    const Bitmap* weekdays = s_settings[LARGE_DATE_FONT] ? s_large_weekdays : s_small_weekdays;
    const Bitmap* bmp = &weekdays[s_weekday];
    draw_bitmap(bmp, (FIELD_WIDTH - bmp->width + 1) / 2, height, color);
}

static void draw_marked_weekday_line(int height, GColor color, int use_letter) {
    const Bitmap* marked_weekdays = s_settings[LARGE_DATE_FONT] ? s_large_marked_weekdays : s_small_marked_weekdays;
    
    const int first_weekday = s_settings[DATE_FIRST_WEEKDAY];
    int width = 0;
    for (int i = 0; i < 7; ++i) {
        const int day = (first_weekday + i) % 7;
        const int bmp_idx = (day == s_weekday) ? (use_letter ? day : 8) : 7;
        width += marked_weekdays[bmp_idx].width;
    }
    int offset = (FIELD_WIDTH - width + 1) / 2;
    for (int i = 0; i < 7; ++i) {
        const int day = (first_weekday + i) % 7;
        const int bmp_idx = (day == s_weekday) ? (use_letter ? day : 8) : 7;
        draw_bitmap_move(&offset, &marked_weekdays[bmp_idx], height, color, 0);
    }
}

static void draw_date_line(int height, GColor color) {
    const DateMonthFormat dmf = s_settings[DATE_MONTH_FORMAT];

    const Bitmap* months = s_settings[LARGE_DATE_FONT] ? s_large_months : s_small_months;
    const Bitmap* weekdays = s_settings[LARGE_DATE_FONT] ? s_large_weekdays : s_small_weekdays;
    const Bitmap* bmp_digits = s_settings[LARGE_DATE_FONT] ? s_bmp_large_digits : s_bmp_small_digits;
    const int bmp_digit_width = s_settings[LARGE_DATE_FONT] ? BMP_LARGE_DIGIT_WIDTH : BMP_SMALL_DIGIT_WIDTH;
    
    // digit
    int width = bmp_digit_width;
    if (s_day >= 10) {
        width += 1 + bmp_digit_width;
    }

    const int date_word_spacing = s_settings[CUSTOM_DATE_WORD_SPACING];

    // month
    switch (dmf) {
    case DMF_MONTH_BEFORE:
    case DMF_MONTH_AFTER:
        width += months[s_month].width + date_word_spacing;
        break;
    case DMF_WEEKDAY_BEFORE:
    case DMF_WEEKDAY_AFTER:
        width += weekdays[s_weekday].width + date_word_spacing;
        break;
    default:
        break;
    }

    int offset = (FIELD_WIDTH - width + 1) / 2;
    
    // month before
    if (dmf == DMF_MONTH_BEFORE) {
        draw_bitmap_move(&offset, &months[s_month], height, color, date_word_spacing);
    } else if (dmf == DMF_WEEKDAY_BEFORE) {
        draw_bitmap_move(&offset, &weekdays[s_weekday], height, color, date_word_spacing);
    }

    // date
    if (s_day >= 10) {
        draw_bitmap_move(&offset, &bmp_digits[s_day / 10], height, color, 1);
    }
    draw_bitmap_move(&offset, &bmp_digits[s_day % 10], height, color, date_word_spacing);

    // month after
    if (dmf == DMF_MONTH_AFTER) {
        draw_bitmap_move(&offset, &months[s_month], height, color, date_word_spacing);
    } else if (dmf == DMF_WEEKDAY_AFTER) {
        draw_bitmap_move(&offset, &weekdays[s_weekday], height, color, date_word_spacing);
    }
}

static int get_final_date_split_height() {
    return s_states[0].offset_y + DIGIT_HEIGHT + s_settings[CUSTOM_TIME_DATE_SPACING_1];
}

static void draw_date() {
    const DateMode dm = s_settings[DATE_MODE];
    if (dm == DM_NONE) {
        return;
    }

    const int date_period_frames = s_settings[CUSTOM_ANIMATION_DATE_PERIOD_FRAMES];
    const int split_height = get_final_date_split_height() + (s_date_frame + date_period_frames - 1) / date_period_frames;

    GColor date_color;
    if (dm == DM_INVERTED) {
        date_color = s_bg_color;
        for (int j = split_height; j < FIELD_HEIGHT; ++j) {
            for (int i = 0; i < FIELD_WIDTH; ++i) {
                field_draw(i, j, s_fg_color);
            }
        }
    } else {
        date_color = s_fg_color;
    }

    const int bmp_height = s_settings[LARGE_DATE_FONT] ? BMP_LARGE_HEIGHT : BMP_SMALL_HEIGHT;
    const int first_line_height = split_height + s_settings[CUSTOM_TIME_DATE_SPACING_2];
    const int second_line_height = first_line_height + bmp_height + s_settings[CUSTOM_DATE_LINE_SPACING];
    const DateWeekdayFormat dwf = s_settings[DATE_WEEKDAY_FORMAT];

    draw_date_line(first_line_height, date_color);
    switch(dwf) {
    case DWF_MARKED:
        draw_marked_weekday_line(second_line_height-1, date_color, 0);
        break;
    case DWF_LETTER:
        draw_marked_weekday_line(second_line_height, date_color, 1);
        break;
    case DWF_TEXT:
        draw_weekday_line(second_line_height, date_color);
        break;
    default:
        break; // nothing
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
                field_draw(x, y, BYTE_TO_COLOR(td->color[tp->color_idx]));
            }
        }
    }
}

static void draw_digit_def(const DigitDef* def, int offset_x, int offset_y) {
    for (int i = 0; i < def->size; ++i) {
        draw_tetrimino(&def->tetriminos[i], offset_x, offset_y);
    }
}

static void draw_digit_state(const DigitState* state) {
    if (state->vanishing_frame) {
        const int animation_period_frames = s_settings[CUSTOM_ANIMATION_PERIOD_VIS_FRAMES] + s_settings[CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES];
        const int in_period = (state->vanishing_frame - 1) % animation_period_frames;
        if (in_period < s_settings[CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES]) {
            return;
        }
    }
    draw_digit_def(&state->current, state->offset_x, state->offset_y);
}

static void layer_draw(Layer* layer, GContext* ctx) {
    for (int i = 0; i < 4; ++i) {
        draw_digit_state(&s_states[i]);
    }
    if (s_show_second_dot || s_states[4].falling || s_states[4].vanishing_frame) {
        draw_digit_state(&s_states[4]);
    }
    draw_date();
    if (s_settings[ICON_CONNECTION]) {
        if (!bluetooth_connection_service_peek()) {
            draw_bitmap(&s_bluetooth, 0, 0, s_fg_color);
        }
    }
    if (s_settings[ICON_BATTERY]) {
        const Bitmap* bmp = 0;
        BatteryChargeState charge_state = battery_state_service_peek();
        if (charge_state.is_charging) {
            bmp = &s_battery_charging;
        } else if (charge_state.charge_percent <= 20) {
            bmp = &s_battery_empty;
        }
        if (bmp) {
            draw_bitmap(bmp, FIELD_WIDTH - bmp->width, 0, s_fg_color);
        }
    }
    
    field_flush(layer, ctx);
}

static int is_animating() {
    if (s_date_frame) {
        return 1;
    }
    for (int i = 0; i < STATE_COUNT; ++i) {
        if (s_states[i].falling || s_states[i].vanishing_frame) {
            return 1;
        }
    }
    return 0;
}

static void process_animation(void* data) {
    /*
    static int st = 0;
    APP_LOG(APP_LOG_LEVEL_INFO, "Step %d", st);
    st += 1;
    */
    
    s_animating = 1;
    if (s_date_frame) {
        s_date_frame -= 1;
    }
    for (int i = 0; i < STATE_COUNT; ++i) {
        state_step(&s_states[i]);
    }
    layer_mark_dirty(s_layer);
    if (is_animating()) {
        app_timer_register(s_settings[CUSTOM_ANIMATION_TIMEOUT_MS], process_animation, NULL);
    } else {
        s_animating = 0;
    }
}

inline static void notify(NotificationType notification) {
    switch (notification) {
    case NTF_SHORT_PULSE:
        vibes_short_pulse();
        break;
    case NTF_LONG_PULSE:
        vibes_long_pulse();
        break;
    case NTF_DOUBLE_PULSE:
        vibes_double_pulse();
        break;
    default:
        break;
    }
}

static void bt_handler(bool connected) {
    if (s_layer && s_settings[ICON_CONNECTION]) {
        layer_mark_dirty(s_layer);
    }
    if (connected) {
        notify(s_settings[NOTIFICATION_CONNECTED]);
    } else {
        notify(s_settings[NOTIFICATION_DISCONNECTED]);
    }
}

static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
    const int clock24 = clock_is_24h_style();
    
    int digit_values[STATE_COUNT];
    int hour = tick_time->tm_hour;
    
    if (!clock24) {
        hour = hour % 12;
        if (hour == 0) {
            hour = 12;
        }
    }
    digit_values[0] = hour / 10;
    digit_values[1] = hour % 10;
    digit_values[2] = tick_time->tm_min / 10;
    digit_values[3] = tick_time->tm_min % 10;
    digit_values[4] = 10;

    if (!clock24 && digit_values[0] == 0) {
        digit_values[0] = DIGIT_COUNT;
    }
    
    int changed = 0;
    for (int i = 0; i < STATE_COUNT; ++i) {
        const int value = digit_values[i];
        if (s_states[i].next_value != value) {
            s_states[i].next_value = value;
            //APP_LOG(APP_LOG_LEVEL_INFO, "Digit %d scheduled to be %d", i, value);
            changed = 1;
        }
    }

    s_month = tick_time->tm_mon;
    s_day = tick_time->tm_mday;
    s_weekday = tick_time->tm_wday;
    //s_weekday = (tick_time->tm_sec / 2) % 7;
    //s_month = ((tick_time->tm_sec + 1) / 2) % 12;

    if (changed && !s_animating) {
        process_animation(NULL);
    }

    if (s_settings[ANIMATE_SECOND_DOT]) {
        s_show_second_dot = tick_time->tm_sec % 2;
        layer_mark_dirty(s_layer);
    }

    if ((units_changed & HOUR_UNIT) && (units_changed != (TimeUnits)(-1))) {
        notify(s_settings[NOTIFICATION_HOURLY]);
    }
}

static void battery_handler(BatteryChargeState charge_state) {
    if (s_layer) {
        layer_mark_dirty(s_layer);
    }
}

static void on_settings_changed() {
    int digits_mode = s_settings[DIGITS_MODE];
    if (digits_mode == DM_ASYMMETRIC && clock_is_24h_style()) {
        digits_mode = DM_DENSE;
    }

    switch (digits_mode) {
    case DM_ASYMMETRIC:
        s_states[0].offset_x = 0;
        s_states[1].offset_x = 8;
        s_states[2].offset_x = 20;
        s_states[3].offset_x = 28;
        s_states[4].offset_x = 14;
        break;
    default:
        APP_LOG(APP_LOG_LEVEL_INFO, "Unsupported digit mode");
        // no break
    case DM_DENSE:
        s_states[0].offset_x = 2;
        s_states[1].offset_x = 10;
        s_states[2].offset_x = 20;
        s_states[3].offset_x = 28;
        s_states[4].offset_x = 15;
        break;
    case DM_SPARSE:
        s_states[0].offset_x = 1;
        s_states[1].offset_x = 9;
        s_states[2].offset_x = 21;
        s_states[3].offset_x = 29;
        s_states[4].offset_x = 15;
        break;
    }

    int offset_y = (FIELD_HEIGHT - DIGIT_HEIGHT) / 2;
    if (s_settings[DATE_MODE] != DM_NONE) {
        offset_y -= s_settings[CUSTOM_TIME_OFFSET];
    }
    for (int i = 0; i < STATE_COUNT; ++i) {
        s_states[i].offset_y = offset_y;
    }

    if (s_settings[LIGHT_THEME]) {
        s_bg_color = GColorWhite;
        s_fg_color = GColorBlack;
    } else {
        s_bg_color = GColorBlack;
        s_fg_color = GColorWhite;
    }

    if (!s_settings[SKIP_INITIAL_ANIMATION]) {
        s_date_frame = (FIELD_HEIGHT - get_final_date_split_height()) * s_settings[CUSTOM_ANIMATION_DATE_PERIOD_FRAMES];
    } else {
        s_date_frame = 0;
    }

    tick_timer_service_unsubscribe();
    if (s_settings[ANIMATE_SECOND_DOT]) {
        tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    } else {
        s_show_second_dot = 1;
        tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    }

    if (s_settings[ICON_CONNECTION] || s_settings[NOTIFICATION_DISCONNECTED] || s_settings[NOTIFICATION_CONNECTED]) {
        bluetooth_connection_service_subscribe(bt_handler);
    } else {
        bluetooth_connection_service_unsubscribe();
    }

    if (s_settings[ICON_BATTERY]) {
        battery_state_service_subscribe(battery_handler);
    } else {
        battery_state_service_unsubscribe();
    }

    field_reset(s_bg_color);
    if (s_layer) {
        layer_mark_dirty(s_layer);
    }
    if (!s_animating) {
        process_animation(NULL);
    }
}

static void in_received_handler(DictionaryIterator* iter, void* context)
{
    settings_read(iter);
    on_settings_changed();
}

static void main_window_load(Window* window) {
    s_layer = window_get_root_layer(window);
    layer_set_update_proc(s_layer, layer_draw);

    on_settings_changed();

    time_t now;
    time(&now);
    struct tm* now_time = localtime(&now);
    tick_handler(now_time, -1);

    if (s_settings[SKIP_INITIAL_ANIMATION]) {
        for (int i = 0; i < STATE_COUNT; ++i) {
            // skip vanishing animation
            const int animation_period_frames = s_settings[CUSTOM_ANIMATION_PERIOD_VIS_FRAMES] + s_settings[CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES];
            s_states[i].vanishing_frame = s_settings[CUSTOM_ANIMATION_PERIOD_COUNT] * animation_period_frames + 1;
            state_step(&s_states[i]);
            s_states[i].current = s_states[i].target;
        }
    }
}

static void main_window_unload(Window* window) {
    s_layer = 0;
}
  
static void init() {
    srand(time(NULL));

    app_message_register_inbox_received(in_received_handler);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    settings_load_persistent();
    
#if USE_RAW_DIGITS == 1
    bitmap_check_all();
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        DigitDef def;
        if (!parse_raw_digit(&def, &s_raw_digits[i])) {
            return;
        }
        reorder_digit(&s_digits[i], &def);
    }
    /*
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        format_digit_def_struct(&s_digits[i]);
    }
    */
#endif

    for (int i = 0; i < STATE_COUNT; ++i) {
        s_states[i].next_value = -1;
        s_states[i].target_value = -1;
        // it looks better WITH vanishing animation
        //s_states[i].vanishing_frame = s_settings[CUSTOM_ANIMATION_PERIOD_COUNT] * ANIMATION_PERIOD_FRAMES + 1;
    }
    s_states[4].restricted_spawn_width = 1;
    
    // init window
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) { .load = main_window_load, .unload = main_window_unload });
    window_stack_push(s_window, true);
}

static void deinit() {
    window_destroy(s_window);
    s_window = 0;
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
