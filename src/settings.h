#include "pebble.h"

typedef enum {
    VERSION = 0,
    LIGHT_THEME,
    ANIMATE_SECOND_DOT,
    DIGITS_MODE,
    DATE_FORMAT,
    MAX_KEY,
} SettingsKey;

typedef enum {
    DATEFMT_HAS_MONTH_BEFORE_DAY = 1,
    DATEFMT_HAS_MONTH_AFTER_DAY  = 2,
    DATEFMT_HAS_WD               = 4,
    DATEFMT_HAS_MONTH_FIRST      = 8,
} DateFormatFlags;

typedef enum {
    DATEFMT_NONE         = 0,
    DATEFMT_MONTH_DAY    = DATEFMT_HAS_MONTH_BEFORE_DAY,
    DATEFMT_DAY_MONTH    = DATEFMT_HAS_MONTH_AFTER_DAY,
    DATEFMT_WD_DAY       = DATEFMT_HAS_WD,
    DATEFMT_WD_MONTH_DAY = DATEFMT_HAS_WD | DATEFMT_HAS_MONTH_BEFORE_DAY,
    DATEFMT_WD_DAY_MONTH = DATEFMT_HAS_WD | DATEFMT_HAS_MONTH_AFTER_DAY,
} DateFormat;

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
        if (t->key < MAX_KEY) {
            switch (t->type) {
            case TUPLE_UINT:
            case TUPLE_INT:
                s_settings[t->key] = t->value->int8;
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Unexpected key type: %d:%d", (int)t->key, (int)t->type);
            }
        } else {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Unexpected key: %d", (int)t->key);
        }
        t = dict_read_next(iter);
    }
    settings_save_persistent();
}
