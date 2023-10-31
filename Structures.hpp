#pragma once

#include <limits>
#include <cstdint>

struct Arguments {
    inline static char* input_file;
    inline static char* output_folder;
    inline static uint64_t max_iter = 0;
    inline static uint64_t freq = 0;
    ~Arguments(){
        delete[] input_file;
        delete[] output_folder;
    }
};

struct Flags {
    inline static bool is_input_file_alone = true;
    inline static bool is_output_file_alone = true;
    inline static bool is_max_iter_alone = true;
    inline static bool is_freq_alone = true;
};

struct Field {
    int16_t x_size = 0;
    int16_t y_size = 0;
    int** main_field;
    ~Field(){
        for(int i = 0; i < x_size; ++i){
            delete[] main_field[i];
        }
        delete[] main_field;
    }
};