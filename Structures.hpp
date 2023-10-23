#pragma once

#include <limits>
#include <cstdint>

struct Arguments{
    inline static char* input_file = new char[1];
    inline static char* output_file = new char[1];
    inline static uint64_t max_iter = 0;
    inline static uint64_t freq = 0;
};

struct Flags{
    inline static bool is_input_file_alone = true;
    inline static bool is_output_file_alone = true;
    inline static bool is_max_iter_alone = true;
    inline static bool is_freq_alone = true;
};

struct Field{
    inline static int16_t x_size = 0;
    inline static int16_t y_size = 0;
    inline static int** field;
};