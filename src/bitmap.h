#define BMP_LARGE_HEIGHT 6
#define BMP_LARGE_DIGIT_WIDTH 4

#define BMP_SMALL_HEIGHT 5
#define BMP_SMALL_DIGIT_WIDTH 3

typedef struct {
    uint8_t width;
    uint8_t height;
    char* data;
} Bitmap;

static const Bitmap s_small_months[] = {
    {
        12, BMP_SMALL_HEIGHT,
        "  # ### #  #"
        "  # # # ## #"
        "  # ### # ##"
        "# # # # #  #"
        "### # # #  #"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### ### ###"
        "#   #   # #"
        "##  ##  ## "
        "#   #   # #"
        "#   ### ###"
    },
    {
        13, BMP_SMALL_HEIGHT,
        "#   # ### ###"
        "## ## # # # #"
        "# # # ### ## "
        "#   # # # # #"
        "#   # # # # #"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### ### ###"
        "# # # # # #"
        "### ### ## "
        "# # #   # #"
        "# # #   # #"
    },
    {
        13, BMP_SMALL_HEIGHT,
        "#   # ### # #"
        "## ## # # # #"
        "# # # ###  # "
        "#   # # #  # "
        "#   # # #  # "
    },
    {
        12, BMP_SMALL_HEIGHT,
        "  # # # #  #"
        "  # # # ## #"
        "  # # # # ##"
        "# # # # #  #"
        "### ### #  #"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "  # # # #  "
        "  # # # #  "
        "  # # # #  "
        "# # # # #  "
        "### ### ###"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### # # ###"
        "# # # # #  "
        "### # # # #"
        "# # # # # #"
        "# # ### ###"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### ### ###"
        "#   #   # #"
        "### ##  ###"
        "  # #   #  "
        "### ### #  "
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### ### ###"
        "# # #    # "
        "# # #    # "
        "# # #    # "
        "### ###  # "
    },
    {
        12, BMP_SMALL_HEIGHT,
        "#  # ### # #"
        "## # # # # #"
        "# ## # # # #"
        "#  # # # # #"
        "#  # ###  # "
    },
    {
        11, BMP_SMALL_HEIGHT,
        "##  ### ###"
        "# # #   #  "
        "# # ##  #  "
        "# # #   #  "
        "##  ### ###"
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_small_months) == 12);

static const Bitmap s_bmp_small_digits[] = {
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "# #"
        "# #"
        "# #"
        "###"
     },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "  #"
        " ##"
        "  #"
        "  #"
        "  #"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "  #"
        "###"
        "#  "
        "###"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "  #"
        " ##"
        "  #"
        "###"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "# #"
        "# #"
        "###"
        "  #"
        "  #"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "#  "
        "###"
        "  #"
        "###"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "#  "
        "###"
        "# #"
        "###"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "  #"
        "  #"
        "  #"
        "  #"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "# #"
        "###"
        "# #"
        "###"
    },
    {
        BMP_SMALL_DIGIT_WIDTH, BMP_SMALL_HEIGHT,
        "###"
        "# #"
        "###"
        "  #"
        "###"
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_bmp_small_digits) == 10);

static const Bitmap s_small_marked_weekdays[] = {
    { 5, 5,
      " ### "
      " #   "
      " ### "
      "   # "
      " ### "
    },
    { 7, 5,
      " #   # "
      " ## ## "
      " # # # "
      " #   # "
      " #   # "
    },
    { 5, 5,
      " ### "
      "  #  "
      "  #  "
      "  #  "
      "  #  "
    },
    { 7, 5,
      " #   # "
      " #   # "
      " #   # "
      " # # # "
      " ## ## "
    },
    { 5, 5,
      " ### "
      "  #  "
      "  #  "
      "  #  "
      "  #  "
    },
    { 5, 5,
      " ### "
      " #   "
      " ##  "
      " #   "
      " #   "
    },
    { 5, 5,
      " ### "
      " #   "
      " ### "
      "   # "
      " ### "
    },
    { 4, 5,
      "    "
      "    "
      " ## "
      " ## "
      "    "
    },
    { 6, 5,
      "      "
      " #### "
      " #  # "
      " #  # "
      " #### "
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_small_marked_weekdays) == 9);

static const Bitmap s_small_weekdays[] = {
    {
        12, BMP_SMALL_HEIGHT,
        "### # # #  #"
        "#   # # ## #"
        "### # # # ##"
        "  # # # #  #"
        "### ### #  #"
    },
    {
        14, BMP_SMALL_HEIGHT,
        "#   # ### #  #"
        "## ## # # ## #"
        "# # # # # # ##"
        "#   # # # #  #"
        "#   # ### #  #"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### # # ###"
        " #  # # #  "
        " #  # # ## "
        " #  # # #  "
        " #  ### ###"
    },
    {
        13, BMP_SMALL_HEIGHT,
        "#   # ### ## "
        "#   # #   # #"
        "#   # ##  # #"
        "# # # #   # #"
        "## ## ### ## "
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### # # # #"
        " #  # # # #"
        " #  ### # #"
        " #  # # # #"
        " #  # # ###"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### ### ###"
        "#   # #  # "
        "##  ##   # "
        "#   # #  # "
        "#   # # ###"
    },
    {
        11, BMP_SMALL_HEIGHT,
        "### ### ###"
        "#   # #  # "
        "### ###  # "
        "  # # #  # "
        "### # #  # "
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_small_weekdays) == 7);

static const Bitmap s_large_months[] = {
    {
        14, BMP_LARGE_HEIGHT,
        "   #          "
        "   #  ##  #  #"
        "   # #  # ## #"
        "   # #### # ##"
        "#  # #  # #  #"
        " ##  #  # #  #"
    },
    {
        13, BMP_LARGE_HEIGHT,
        "####         "
        "#    ### ### "
        "###  #   #  #"
        "#    ##  ### "
        "#    #   #  #"
        "#    ### ### "
    },
    {
        15, BMP_LARGE_HEIGHT,
        "#   #          "
        "## ##  ##  ### "
        "# # # #  # #  #"
        "#   # #### ### "
        "#   # #  # #  #"
        "#   # #  # #  #"
    },
    {
        14, BMP_LARGE_HEIGHT,
        " ##           "
        "#  # ###  ### "
        "#  # #  # #  #"
        "#### ###  ### "
        "#  # #    #  #"
        "#  # #    #  #"
    },
    {
        14, BMP_LARGE_HEIGHT,
        "#   #         "
        "## ##  ##  # #"
        "# # # #  # # #"
        "#   # ####  # "
        "#   # #  #  # "
        "#   # #  #  # "
    },
    {
        14, BMP_LARGE_HEIGHT,
        "   #          "
        "   # #  # #  #"
        "   # #  # ## #"
        "   # #  # # ##"
        "#  # #  # #  #"
        " ##   ##  #  #"
    },
    {
        13, BMP_LARGE_HEIGHT,
        "   #         "
        "   # #  # #  "
        "   # #  # #  "
        "   # #  # #  "
        "#  # #  # #  "
        " ##   ##  ###"
    },
    {
        14, BMP_LARGE_HEIGHT,
        " ##           "
        "#  # #  #  ## "
        "#  # #  # #  #"
        "#### #  # #   "
        "#  # #  # # ##"
        "#  #  ##   ###"
    },
    {
        13, BMP_LARGE_HEIGHT,
        " ##          "
        "#  # ### ### "
        " #   #   #  #"
        "  #  ##  ### "
        "#  # #   #   "
        " ##  ### #   "
    },
    {
        13, BMP_LARGE_HEIGHT,
        " ##          "
        "#  #  ##  ###"
        "#  # #  #  # "
        "#  # #     # "
        "#  # #  #  # "
        " ##   ##   # "
    },
    {
        13, BMP_LARGE_HEIGHT,
        "#  #         "
        "## #  ##  # #"
        "#### #  # # #"
        "# ## #  # # #"
        "#  # #  # # #"
        "#  #  ##   # "
    },
    {
        13, BMP_LARGE_HEIGHT,
        "###          "
        "#  # ###  ## "
        "#  # #   #  #"
        "#  # ##  #   "
        "#  # #   #  #"
        "###  ###  ## "
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_large_months) == 12);

static const Bitmap s_bmp_large_digits[] = {
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        " ## "
        "#  #"
        "#  #"
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        "  # "
        " ## "
        "  # "
        "  # "
        "  # "
        " ###"
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        " ## "
        "#  #"
        "   #"
        " ## "
        "#   "
        "####"
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        " ## "
        "#  #"
        "  # "
        "   #"
        "#  #"
        " ## "
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        "#  #"
        "#  #"
        "####"
        "   #"
        "   #"
        "   #"
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        "####"
        "#   "
        "### "
        "   #"
        "   #"
        "### "
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        " ## "
        "#   "
        "### "
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        "####"
        "   #"
        "  # "
        "  # "
        "  # "
        "  # "
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        " ## "
        "#  #"
        " ## "
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_LARGE_DIGIT_WIDTH, BMP_LARGE_HEIGHT,
        " ## "
        "#  #"
        "#  #"
        " ###"
        "   #"
        " ## "
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_bmp_large_digits) == 10);

static const Bitmap s_large_marked_weekdays[9] = {
    { 6, 5,
      "  ### "
      " #    "
      "  ##  "
      "    # "
      " ###  "
    },
    { 7, 5,
      " #   # "
      " ## ## "
      " # # # "
      " #   # "
      " #   # "
    },
    { 5, 5,
      "#####"
      "  #  "
      "  #  "
      "  #  "
      "  #  "
    },
    { 7, 5,
      " #   # "
      " #   # "
      " #   # "
      " # # # "
      "  # #  "
    },
    { 5, 5,
      "#####"
      "  #  "
      "  #  "
      "  #  "
      "  #  "
    },
    { 6, 5,
      " #### "
      " #    "
      " ###  "
      " #    "
      " #    "
    },
    { 6, 5,
      "  ### "
      " #    "
      "  ##  "
      "    # "
      " ###  "
    },
    { 4, 5,
      "    "
      "    "
      " ## "
      " ## "
      "    "
    },
    { 6, 5,
      "      "
      " #### "
      " #  # "
      " #  # "
      " #### "
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_large_marked_weekdays) == 9);

static const Bitmap s_large_weekdays[] = {
    {
        14, BMP_LARGE_HEIGHT,
        " ##           "
        "#  # #  # #  #"
        " #   #  # ## #"
        "  #  #  # # ##"
        "#  # #  # #  #"
        " ##   ##  #  #"
    },
    {
        15, BMP_LARGE_HEIGHT,
        "#   #          "
        "## ##  ##  #  #"
        "# # # #  # ## #"
        "#   # #  # # ##"
        "#   # #  # #  #"
        "#   #  ##  #  #"
    },
    {
        13, BMP_LARGE_HEIGHT,
        "#####        "
        "  #  #  # ###"
        "  #  #  # #  "
        "  #  #  # ## "
        "  #  #  # #  "
        "  #   ##  ###"
    },
    {
        14, BMP_LARGE_HEIGHT,
        "#   #         "
        "#   # ### ### "
        "#   # #   #  #"
        "# # # ##  #  #"
        "# # # #   #  #"
        " # #  ### ### "
    },
    {
        14, BMP_LARGE_HEIGHT,
        "#####         "
        "  #  #    #  #"
        "  #  #    #  #"
        "  #  ###  #  #"
        "  #  #  # #  #"
        "  #  #  #  ## "
    },
    {
        13, BMP_LARGE_HEIGHT,
        "####         "
        "#    ###  ###"
        "###  #  #  # "
        "#    ###   # "
        "#    #  #  # "
        "#    #  # ###"
    },
    {
        13, BMP_LARGE_HEIGHT,
        " ##          "
        "#  #  ##  ###"
        " #   #  #  # "
        "  #  ####  # "
        "#  # #  #  # "
        " ##  #  #  # "
    },
};
STATIC_ASSERT(ARRAY_SIZE(s_large_weekdays) == 7);

static const Bitmap s_bluetooth = {
    4, 5,
    "  # "
    "  ##"
    " ## "
    "  ##"
    "  # "
};

static const Bitmap s_battery_empty = {
    6, 4,
    "      "
    "####  "
    "#  ## "
    "####  "
};

static const Bitmap s_battery_charging = {
    6, 4,
    "      "
    "####  "
    "##### "
    "####  "
};


static void draw_bitmap(const Bitmap* bmp, int x, int y, GColor color) {
    int yoffset = 0;
    for (int j = 0; j < bmp->height; ++j) {
        for (int i = 0; i < bmp->width; ++i) {
            if (bmp->data[yoffset + i] != ' ') {
                field_draw(x+i, y+j, color);
            }
        }
        yoffset += bmp->width;
    }
}

static void draw_bitmap_move(int* xptr, const Bitmap* bmp, int y, GColor color, int spacing) {
    draw_bitmap(bmp, *xptr, y, color);
    *xptr += bmp->width + spacing;
}

static void bitmap_check(const Bitmap* bmp, const char* label) {
    if ((int)strlen(bmp->data) != bmp->width * bmp->height) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Bitmap %s size mismatch: %dx%d with data lenght %d", label, bmp->width, bmp->height, strlen(bmp->data));
    }
}

#define BITMAP_CHECK_ARRAY(array) for (unsigned int i = 0; i < ARRAY_SIZE(array); ++i) { bitmap_check(&array[i], #array); }

static void bitmap_check_all() {
    APP_LOG(APP_LOG_LEVEL_INFO, "Bitmap check");
    BITMAP_CHECK_ARRAY(s_small_months);
    BITMAP_CHECK_ARRAY(s_large_months);
    BITMAP_CHECK_ARRAY(s_bmp_small_digits);
    BITMAP_CHECK_ARRAY(s_bmp_large_digits);
    BITMAP_CHECK_ARRAY(s_small_marked_weekdays);
    BITMAP_CHECK_ARRAY(s_large_marked_weekdays);
    BITMAP_CHECK_ARRAY(s_small_weekdays);
    BITMAP_CHECK_ARRAY(s_large_weekdays);
    bitmap_check(&s_bluetooth, "bt");
    bitmap_check(&s_battery_empty, "bat_emp");
    bitmap_check(&s_battery_charging, "bat_char");
    APP_LOG(APP_LOG_LEVEL_INFO, "Bitmap done");
}
