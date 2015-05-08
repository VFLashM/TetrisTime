#include "pebble.h"

#define SETTINGS_VERSION_VALUE 2

// I should've used a prefix =/
typedef enum {
    VERSION = 0,
    LIGHT_THEME,
    ANIMATE_SECOND_DOT,
    DIGITS_MODE,

    DATE_MODE,
    DATE_MONTH_FORMAT,
    DATE_WEEKDAY_FORMAT,
    DATE_FIRST_WEEKDAY,

    ICON_CONNECTION,
    ICON_BATTERY,
    
    NOTIFICATION_DISCONNECTED,
    NOTIFICATION_CONNECTED,

    SKIP_INITIAL_ANIMATION,
    NOTIFICATION_HOURLY,

    LARGE_DATE_FONT,

    CUSTOM_DATE,
    CUSTOM_TIME_OFFSET,
    CUSTOM_TIME_DATE_SPACING_1,
    CUSTOM_TIME_DATE_SPACING_2,
    CUSTOM_DATE_WORD_SPACING,
    CUSTOM_DATE_LINE_SPACING,
    CUSTOM_DATE_MAX,

    CUSTOM_ANIMATIONS = CUSTOM_DATE_MAX,
    CUSTOM_ANIMATION_TIMEOUT_MS,
    CUSTOM_ANIMATION_PERIOD_VIS_FRAMES,
    CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES,
    CUSTOM_ANIMATION_PERIOD_COUNT,
    CUSTOM_ANIMATION_DATE_PERIOD_FRAMES,
    CUSTOM_ANIMATIONS_MAX,
    
    MAX_KEY = CUSTOM_ANIMATIONS_MAX,
} SettingsKey;

typedef enum {
    DM_ASYMMETRIC,
    DM_DENSE,
    DM_SPARSE,
    DIM_MAX,
} DigitsMode;

typedef enum {
    DM_INVERTED,
    DM_SAME_COLOR,
    DM_NONE,
    DM_MAX,
} DateMode;

typedef enum {
    DMF_MONTH_BEFORE,
    DMF_MONTH_AFTER,
    DMF_WEEKDAY_BEFORE,
    DMF_WEEKDAY_AFTER,
    DMF_MAX,
} DateMonthFormat;

typedef enum {
    DWF_MARKED,
    DWF_LETTER,
    DWF_TEXT,
    DWF_NO_WEEKDAY,
    DWF_MAX,
} DateWeekdayFormat;

typedef enum {
    NTF_NONE,
    NTF_SHORT_PULSE,
    NTF_LONG_PULSE,
    NTF_DOUBLE_PULSE,
    NTF_MAX,
} NotificationType;

typedef int Settings[MAX_KEY];

static Settings s_settings;

inline static int settings_get_default(SettingsKey key) {
    switch (key) {
    case ANIMATE_SECOND_DOT:
        return 1;
    case DATE_WEEKDAY_FORMAT:
        return DWF_TEXT;
    case NOTIFICATION_DISCONNECTED:
        return NTF_DOUBLE_PULSE;
    default:
        return 0;
    }
}

#define MAKE_IN_RANGE(v, min, max) v = (v < min ? min : (v > max ? max : v))

// returns true if resulting settings differ from input settings
static int settings_apply(const int* new_settings) {
    memcpy(&s_settings, new_settings, sizeof(Settings));
    
    s_settings[VERSION] = SETTINGS_VERSION_VALUE;
    s_settings[LIGHT_THEME] %= 2;
    s_settings[ANIMATE_SECOND_DOT] %= 2;
    s_settings[DIGITS_MODE] %= DIM_MAX;
    s_settings[DATE_MODE] %= DM_MAX;
    s_settings[DATE_MONTH_FORMAT] %= DMF_MAX;
    s_settings[DATE_WEEKDAY_FORMAT] %= DWF_MAX;
    s_settings[DATE_FIRST_WEEKDAY] %= 7;
    s_settings[ICON_CONNECTION] %= 2;
    s_settings[ICON_BATTERY] %= 2;
    s_settings[NOTIFICATION_DISCONNECTED] %= NTF_MAX;
    s_settings[NOTIFICATION_CONNECTED] %= NTF_MAX;
    s_settings[SKIP_INITIAL_ANIMATION] %= 2;
    s_settings[NOTIFICATION_HOURLY] %= NTF_MAX;
    s_settings[LARGE_DATE_FONT] %= 2;
    s_settings[CUSTOM_DATE] %= 2;
    s_settings[CUSTOM_ANIMATIONS] %= 2;

    // disable duplicated text weekday
    if (s_settings[DATE_WEEKDAY_FORMAT] == DWF_TEXT) {
        const DateMonthFormat dmf = s_settings[DATE_MONTH_FORMAT];
        if (dmf == DMF_WEEKDAY_BEFORE || dmf == DMF_WEEKDAY_AFTER) {
            s_settings[DATE_WEEKDAY_FORMAT] = DWF_NO_WEEKDAY;
        }
    }

    // fix custom values
    if (!s_settings[CUSTOM_DATE]) {
        if (s_settings[DATE_MODE] != DM_NONE) {
            const int large_font = s_settings[LARGE_DATE_FONT];
            switch (s_settings[DATE_WEEKDAY_FORMAT]) {
            case DWF_MARKED:
                s_settings[CUSTOM_TIME_OFFSET] = large_font ? 2 : 1;
                break;
            case DWF_LETTER:
                s_settings[CUSTOM_TIME_OFFSET] = large_font ? 3 : 2;
                break;
            case DWF_TEXT:
                s_settings[CUSTOM_TIME_OFFSET] = large_font ? 4 : 2;
                break;
            default:
            case DWF_NO_WEEKDAY:
                s_settings[CUSTOM_TIME_OFFSET] = 1;
                break;
            }
        } else {
            s_settings[CUSTOM_TIME_OFFSET] = 0;
        }
        s_settings[CUSTOM_TIME_DATE_SPACING_1] = 2;
        s_settings[CUSTOM_TIME_DATE_SPACING_2] = 2;
        s_settings[CUSTOM_DATE_WORD_SPACING] = 3;
        s_settings[CUSTOM_DATE_LINE_SPACING] = 2;
    } else {
        MAKE_IN_RANGE(s_settings[CUSTOM_TIME_OFFSET], 0, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_TIME_DATE_SPACING_1], 0, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_TIME_DATE_SPACING_2], 0, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_DATE_WORD_SPACING], 0, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_DATE_LINE_SPACING], 0, 20);
    }

    if (!s_settings[CUSTOM_ANIMATIONS]) {
        s_settings[CUSTOM_ANIMATION_TIMEOUT_MS] = 100;
        s_settings[CUSTOM_ANIMATION_PERIOD_VIS_FRAMES] = 2;
        s_settings[CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES] = 1;
        s_settings[CUSTOM_ANIMATION_PERIOD_COUNT] = 3;
        s_settings[CUSTOM_ANIMATION_DATE_PERIOD_FRAMES] = 4;
    } else {
        MAKE_IN_RANGE(s_settings[CUSTOM_ANIMATION_TIMEOUT_MS], 10, 10000);
        MAKE_IN_RANGE(s_settings[CUSTOM_ANIMATION_PERIOD_VIS_FRAMES], 1, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_ANIMATION_PERIOD_INVIS_FRAMES], 1, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_ANIMATION_PERIOD_COUNT], 0, 20);
        MAKE_IN_RANGE(s_settings[CUSTOM_ANIMATION_DATE_PERIOD_FRAMES], 1, 20);
    }

    /*
    for (int i = 0; i < MAX_KEY; ++i) {
        if (s_settings[i] != new_settings[i]) {
            APP_LOG(APP_LOG_LEVEL_INFO, "Applied %d=%d (was %d)", i, s_settings[i], new_settings[i]);
        }
    }
    */
    
    for (int i = 0; i < MAX_KEY; ++i) {
        if (s_settings[i] != new_settings[i]) {
            return 1;
        }
    }
    return 0;
}

inline static int settings_is_active(const int* settings, SettingsKey idx) {
    if ((idx > CUSTOM_DATE) && (idx < CUSTOM_DATE_MAX)) {
        return settings[CUSTOM_DATE];
    }
    if ((idx > CUSTOM_ANIMATIONS) && (idx < CUSTOM_ANIMATIONS_MAX)) {
        return settings[CUSTOM_ANIMATIONS];
    }
    return 1;
}

static void settings_save_persistent() {
    APP_LOG(APP_LOG_LEVEL_INFO, "Saving persistent settings");
    for (int i = 0; i < MAX_KEY; ++i) {
        if (settings_is_active(s_settings, i)) {
            persist_write_int(i, s_settings[i]);
        } 
    }
}

static void settings_send() {
    // sending settings breaks app_message, disabled until resolved
    return;
    
    APP_LOG(APP_LOG_LEVEL_INFO, "Sending settings");
    DictionaryIterator* it;
    AppMessageResult rc = app_message_outbox_begin(&it);
    if (rc != APP_MSG_OK) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to open outbox, rc=%d", (int)rc);
        return;
    }

    for (int i = 0; i < MAX_KEY; ++i) {
        dict_write_int(it, i, &s_settings[i], 4, 1);
    }
 
    app_message_outbox_send();
}

static void settings_load_persistent() {
    APP_LOG(APP_LOG_LEVEL_INFO, "Reading persistent settings");
    Settings new_settings;
    for (int i = 0; i < MAX_KEY; ++i) {
        new_settings[i] = persist_read_int(i);
        int df = settings_get_default(i);
        APP_LOG(APP_LOG_LEVEL_INFO, "Got default %d=%d %d %d", i, new_settings[i], persist_exists(i), df);
        
        /*
        if (settings_is_active(new_settings, i) && persist_exists(i)) {
            new_settings[i] = persist_read_int(i);
            APP_LOG(APP_LOG_LEVEL_INFO, "Got loaded %d=%d", i, new_settings[i]);
            } else {
        new_settings[i] = settings_get_default(i);
        APP_LOG(APP_LOG_LEVEL_INFO, "Got default %d=%d", i, new_settings[i]);
            }*/
    }

    //if (settings_apply(new_settings)) {
        //settings_save_persistent();
        //settings_send();
    //}
}

static void settings_read(DictionaryIterator* iter) 
{
    APP_LOG(APP_LOG_LEVEL_INFO, "Reading js settings");
    Settings new_settings;
    memcpy(&new_settings, &s_settings, sizeof(Settings));
    
    const Tuple* t = dict_read_first(iter);
    while (t) {
        if (t->key < MAX_KEY) {
            switch (t->type) {
            case TUPLE_UINT:
            case TUPLE_INT:
                new_settings[t->key] = t->value->int8;
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_ERROR, "Unexpected key type: %d:%d", (int)t->key, (int)t->type);
            }
        } else {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Unexpected key: %d", (int)t->key);
        }
        t = dict_read_next(iter);
    }

    if (settings_apply(new_settings)) {
        settings_send();
    }
    settings_save_persistent();
}
