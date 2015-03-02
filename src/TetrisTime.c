#include <pebble.h>
#include "digit.h"
  
#define ANIMATION_TIMEOUT_MS 100
  
#define FIELD_WIDTH 16
#define FIELD_HEIGHT 16
#define CELL_SIZE 3
#define CELL_SPACING 1
  
  
bool s_field[FIELD_WIDTH][FIELD_HEIGHT];
  
static Window *s_window;
static Layer *s_layer;


static void layer_draw(Layer *layer, GContext *ctx) {
    //GRect bounds = layer_get_bounds(layer);  
  
    GRect rect;
    rect.size.h = CELL_SIZE;
    rect.size.w = CELL_SIZE;
  
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {      
            rect.origin.x = CELL_SPACING + i * (CELL_SIZE + CELL_SPACING);
            rect.origin.y = CELL_SPACING + j * (CELL_SIZE + CELL_SPACING);
            GColor color = s_field[i][j] ? GColorBlack : GColorWhite;
            graphics_context_set_fill_color(ctx, color);
            graphics_fill_rect(ctx, rect, 0, GCornerNone);
        }
    }  
}

static void update_time() {
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            s_field[i][j] = rand() % 2 == 0;
        }
    }
    layer_mark_dirty(s_layer);
  
    //window_get_root_layer(window);
  
    /*
    // Get a tm structure
    time_t temp = time(NULL); 
    struct tm *tick_time = localtime(&temp);

    // Create a long-lived buffer
    static char buffer[] = "00:00";

    // Write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    }

    // Display this time on the TextLayer
    text_layer_set_text(s_time_layer, buffer);
    */
}

static void process_animation(void* data) {
    update_time();
    app_timer_register(ANIMATION_TIMEOUT_MS, process_animation, NULL);
}

static void main_window_load(Window *window) {
    // Create time TextLayer
    s_layer = window_get_root_layer(window);
    layer_set_update_proc(s_layer, layer_draw);
  
    // Make sure the time is displayed from the start
    update_time();
  
}

static void main_window_unload(Window *window) {
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}
  
static void init() {
#if USE_RAW_DIGITS == 1
    for (int i = 0; i < 1; ++i) {
    if (!parse_raw_digit(&s_digits[i], &s_raw_digits[i])) {
    return;
}
}
#endif 
  
    // Create main Window element and assign to pointer
    s_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_window, (WindowHandlers) {
            .load = main_window_load,
                .unload = main_window_unload
                });

    // Show the Window on the watch, with animated=true
    window_stack_push(s_window, true);
  
    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    app_timer_register(ANIMATION_TIMEOUT_MS, process_animation, NULL);
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
