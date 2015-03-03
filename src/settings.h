#include "pebble.h"

typedef enum {
    VERSION = 0,
    DARK_THEME,
    ANIMATE_SECOND_DOT,
    COMPACT_DIGITS,
    MAX_KEY,
} SettingsKey;

typedef int Settings[MAX_KEY];

static Settings s_settings;

static void settings_load_persistent() {
    for (int i = 0; i < MAX_KEY; ++i) {
        s_settings[i] = persist_read_int(i);
    }
}

static void settings_save_persistent() {
    for (int i = 0; i < MAX_KEY; ++i) {
        persist_write_int(i, s_settings[i]);
    }
}

static void settings_send() {
    DictionaryIterator* it;
    app_message_outbox_begin(&it);

    for (int i = 0; i < MAX_KEY; ++i) {
        dict_write_int(it, i, &s_settings[i], 4, 1);
    }
 
    app_message_outbox_send();
}

static void settings_read(DictionaryIterator* iter) 
{
    Tuple* t = dict_read_first(iter);
    while (t) {
        s_settings[t->key] = t->value->int32;
        t = dict_read_next(iter);
    }
    settings_save_persistent();
}
