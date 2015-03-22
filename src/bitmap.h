#define BMP_HEIGHT 6
#define BMP_DIGIT_WIDTH 4

typedef struct {
    int width;
    int height;
    char* data;
} Bitmap;

static const Bitmap s_months[12] = {
    {
        14, BMP_HEIGHT,
        "   #          "
        "   #  ##  #  #"
        "   # #  # ## #"
        "   # #### # ##"
        "#  # #  # #  #"
        " ##  #  # #  #"
    },
    {
        13, BMP_HEIGHT,
        "####         "
        "#    ### ### "
        "###  #   #  #"
        "#    ##  ### "
        "#    #   #  #"
        "#    ### ### "
    },
    {
        15, BMP_HEIGHT,
        "#   #          "
        "## ##  ##  ### "
        "# # # #  # #  #"
        "#   # #### ### "
        "#   # #  # #  #"
        "#   # #  # #  #"
    },
    {
        14, BMP_HEIGHT,
        " ##           "
        "#  # ###  ### "
        "#  # #  # #  #"
        "#### ###  ### "
        "#  # #    #  #"
        "#  # #    #  #"
    },
    {
        14, BMP_HEIGHT,
        "#   #         "
        "## ##  ##  # #"
        "# # # #  # # #"
        "#   # ####  # "
        "#   # #  #  # "
        "#   # #  #  # "
    },
    {
        14, BMP_HEIGHT,
        "   #          "
        "   # #  # #  #"
        "   # #  # ## #"
        "   # #  # # ##"
        "#  # #  # #  #"
        " ##   ##  #  #"
    },
    {
        13, BMP_HEIGHT,
        "   #         "
        "   # #  # #  "
        "   # #  # #  "
        "   # #  # #  "
        "#  # #  # #  "
        " ##   ##  ###"
    },
    {
        14, BMP_HEIGHT,
        " ##           "
        "#  # #  #  ## "
        "#  # #  # #  #"
        "#### #  # #   "
        "#  # #  # # ##"
        "#  #  ##   ###"
    },
    {
        13, BMP_HEIGHT,
        " ##          "
        "#  # ### ### "
        " #   #   #  #"
        "  #  ##  ### "
        "#  # #   #   "
        " ##  ### #   "
    },
    {
        13, BMP_HEIGHT,
        " ##          "
        "#  #  ##  ###"
        "#  # #  #  # "
        "#  # #     # "
        "#  # #  #  # "
        " ##   ##   # "
    },
    {
        13, BMP_HEIGHT,
        "#  #         "
        "## #  ##  # #"
        "#### #  # # #"
        "# ## #  # # #"
        "#  # #  # # #"
        "#  #  ##   # "
    },
    {
        13, BMP_HEIGHT,
        "###          "
        "#  # ###  ## "
        "#  # #   #  #"
        "#  # ##  #   "
        "#  # #   #  #"
        "###  ###  ## "
    },
};

static const Bitmap s_bmp_digits[10] = {
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        " ## "
        "#  #"
        "#  #"
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        "  # "
        " ## "
        "  # "
        "  # "
        "  # "
        " ###"
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        " ## "
        "#  #"
        "   #"
        " ## "
        "#   "
        "####"
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        " ## "
        "#  #"
        "  # "
        "   #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        "#  #"
        "#  #"
        "####"
        "   #"
        "   #"
        "   #"
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        "####"
        "#   "
        "### "
        "   #"
        "   #"
        "### "
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        " ## "
        "#   "
        "### "
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        "####"
        "   #"
        "  # "
        "  # "
        "  # "
        "  # "
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        " ## "
        "#  #"
        " ## "
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, BMP_HEIGHT,
        " ## "
        "#  #"
        "#  #"
        " ###"
        "   #"
        " ## "
    },
};

static const Bitmap s_marked_weekdays[9] = {
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

static const Bitmap s_weekdays[7] = {
    {
        14, BMP_HEIGHT,
        " ##           "
        "#  # #  # #  #"
        " #   #  # ## #"
        "  #  #  # # ##"
        "#  # #  # #  #"
        " ##   ##  #  #"
    },
    {
        15, BMP_HEIGHT,
        "#   #          "
        "## ##  ##  #  #"
        "# # # #  # ## #"
        "#   # #  # # ##"
        "#   # #  # #  #"
        "#   #  ##  #  #"
    },
    {
        13, BMP_HEIGHT,
        "#####        "
        "  #  #  # ###"
        "  #  #  # #  "
        "  #  #  # ## "
        "  #  #  # #  "
        "  #   ##  ###"
    },
    {
        14, BMP_HEIGHT,
        "#   #         "
        "#   # ### ### "
        "#   # #   #  #"
        "# # # ##  #  #"
        "# # # #   #  #"
        " # #  ### ### "
    },
    {
        14, BMP_HEIGHT,
        "#####         "
        "  #  #    #  #"
        "  #  #    #  #"
        "  #  ###  #  #"
        "  #  #  # #  #"
        "  #  #  #  ## "
    },
    {
        13, BMP_HEIGHT,
        "####         "
        "#    ###  ###"
        "###  #  #  # "
        "#    ###   # "
        "#    #  #  # "
        "#    #  # ###"
    },
    {
        13, BMP_HEIGHT,
        " ##          "
        "#  #  ##  ###"
        " #   #  #  # "
        "  #  ####  # "
        "#  # #  #  # "
        " ##  #  #  # "
    },
};

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


static void draw_bitmap(const Bitmap* bmp, int x, int y, PaletteColor color) {
    int yoffset = 0;
    for (int j = 0; j < bmp->height; ++j) {
        for (int i = 0; i < bmp->width; ++i) {
            if (bmp->data[yoffset + i] != ' ') {
                field_draw(x+i, y+j, color);
            }
        }
        yoffset += bmp->width;
    }
    // if ((int)strlen(bmp->data) != bmp->width * bmp->height) {
    //     APP_LOG(APP_LOG_LEVEL_ERROR, "Bitmap size mismatch: %dx%d with data lenght %d", bmp->width, bmp->height, strlen(bmp->data));
    // }
}

static void draw_bitmap_move(int* xptr, const Bitmap* bmp, int y, PaletteColor color, int spacing) {
    draw_bitmap(bmp, *xptr, y, color);
    *xptr += bmp->width + spacing;
}
