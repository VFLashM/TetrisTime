#define MONTH_WIDTH 13
#define BMP_DIGIT_WIDTH 4

typedef struct {
    int width;
    int height;
    char* data;
} Bitmap;

static Bitmap s_months[12] = {
    {
        MONTH_WIDTH, 6,
        "   #         "
        "   #  #  #  #"
        "   # # # ## #"
        "   # ### # ##"
        "#  # # # #  #"
        " ##  # # #  #"
    },
    {
        MONTH_WIDTH, 6,
        "####         "
        "#    ### ### "
        "###  #   #  #"
        "#    ##  ### "
        "#    #   #  #"
        "#    ### ### "
    },
    {
        MONTH_WIDTH, 6,
        "#   #        "
        "## ##  #  ## "
        "# # # # # # #"
        "#   # ### ## "
        "#   # # # # #"
        "#   # # # # #"
    },
    {
        MONTH_WIDTH, 6,
        " ##          "
        "#  # ###  ## "
        "#  # #  # # #"
        "#### ###  ## "
        "#  # #    # #"
        "#  # #    # #"
    },
    {
        MONTH_WIDTH, 6,
        "#   #        "
        "## ##  #  # #"
        "# # # # # # #"
        "#   # ###  # "
        "#   # # #  # "
        "#   # # #  # "
    },
    {
        MONTH_WIDTH, 6,
        "   #         "
        "   # # # #  #"
        "   # # # ## #"
        "   # # # # ##"
        "#  # # # #  #"
        " ##   #  #  #"
    },
    {
        MONTH_WIDTH, 6,
        "   #         "
        "   # #  # #  "
        "   # #  # #  "
        "   # #  # #  "
        "#  # #  # #  "
        " ##   ##  ###"
    },
    {
        MONTH_WIDTH, 6,
        " ##          "
        "#  # # #  ## "
        "#  # # # #  #"
        "#### # # #   "
        "#  # # # # ##"
        "#  #  #   ###"
    },
    {
        MONTH_WIDTH, 6,
        " ##          "
        "#  # ### ### "
        " #   #   #  #"
        "  #  ##  ### "
        "#  # #   #   "
        " ##  ### #   "
    },
    {
        MONTH_WIDTH, 6,
        " ##          "
        "#  #  ##  ###"
        "#  # #  #  # "
        "#  # #     # "
        "#  # #  #  # "
        " ##   ##   # "
    },
    {
        MONTH_WIDTH, 6,
        "#  #         "
        "## #  ##  # #"
        "## # #  # # #"
        "# ## #  # # #"
        "# ## #  # # #"
        "#  #  ##   # "
    },
    {
        MONTH_WIDTH, 6,
        "###          "
        "#  # ###  ## "
        "#  # #   #  #"
        "#  # ##  #   "
        "#  # #   #  #"
        "###  ###  ## "
    },
};

static Bitmap s_bmp_digits[10] = {
    {
        BMP_DIGIT_WIDTH, 6,
        " ## "
        "#  #"
        "#  #"
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, 6,
        "  # "
        " ## "
        "  # "
        "  # "
        "  # "
        " ###"
    },
    {
        BMP_DIGIT_WIDTH, 6,
        " ## "
        "#  #"
        "   #"
        " ## "
        "#   "
        "####"
    },
    {
        BMP_DIGIT_WIDTH, 6,
        " ## "
        "#  #"
        "  # "
        "   #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, 6,
        "#  #"
        "#  #"
        "####"
        "   #"
        "   #"
        "   #"
    },
    {
        BMP_DIGIT_WIDTH, 6,
        "####"
        "#   "
        "### "
        "   #"
        "   #"
        "### "
    },
    {
        BMP_DIGIT_WIDTH, 6,
        " ## "
        "#   "
        "### "
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, 6,
        "####"
        "   #"
        "  # "
        "  # "
        "  # "
        "  # "
    },
    {
        BMP_DIGIT_WIDTH, 6,
        " ## "
        "#  #"
        " ## "
        "#  #"
        "#  #"
        " ## "
    },
    {
        BMP_DIGIT_WIDTH, 6,
        " ## "
        "#  #"
        "#  #"
        " ###"
        "   #"
        " ## "
    },
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
}

static void draw_bitmap_move(int* xptr, const Bitmap* bmp, int y, PaletteColor color, int spacing) {
    draw_bitmap(bmp, *xptr, y, color);
    *xptr += bmp->width + spacing;
}
