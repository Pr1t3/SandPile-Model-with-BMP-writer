#pragma once

#include <limits>

struct Arguments{
    char* input_file = new char[0];
    char* output_file = new char[0];
    uint64_t max_iter;
    uint64_t freq;
};

struct Flags{
    bool is_input_file_alone = true;
    bool is_output_file_alone = true;
    bool is_max_iter_alone = true;
    bool is_freq_alone = true;
};