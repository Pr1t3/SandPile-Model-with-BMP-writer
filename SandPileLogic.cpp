#include "SandPileLogic.h"

bool OpenFile(std::ifstream& file) {
    file.open(Arguments::input_file);
    if (!file.is_open()) {
        std::cerr << "File not found";
        return false;
    }
    return true;
}

void FillFieldOnStart(std::ifstream& file) {
    bool is_first = true;
    bool is_second = false;
    while (file) {
        char symbol = file.get();
        if (symbol == EOF) {
            break;
        }
        if (symbol == '\r' || symbol == '\t' || symbol == '\n') {
            continue;
        }
        int num = static_cast<int>(symbol - '0');
        if (is_first) {
            if (Field::x_size < num) {
                Field::x_size = num;
            }
            is_first = false;
            is_second = true;
        } else if (is_second) {
            if (Field::y_size < num) {
                Field::y_size = num;
            }
            is_second = false;
        } else {
            is_first = true;
            is_second = false;
        }
    }
    ++Field::y_size;
    ++Field::x_size;
    Field::field = new int* [Field::y_size];
    for (int i = 0; i < Field::y_size; ++i) {
        Field::field[i] = new int[Field::x_size];
        for(int j = 0; j < Field::x_size; ++j){
            Field::field[i][j] = 0;
        }
    }
    file.clear();
    file.seekg(0);
    int x = -1;
    int y = -1;
    while (file) {
        char symbol = file.get();
        if (symbol == EOF) {
            break;
        }
        if (symbol == '\r' || symbol == '\t' || symbol == '\n') {
            continue;
        }
        int num = static_cast<int>(symbol - '0');
        if (x == -1) {
            x = num;
        } else if (y == -1) {
            y = num;
        } else {
            Field::field[y][x] = num;
            x = -1;
            y = -1;
        }
    }
}

void PrintField(int** field){
    for (int16_t i = 0; i < Field::y_size; ++i) {
        for (int16_t j = 0; j < Field::x_size; ++j) {
            std::cout << field[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int** ResizeField(int** field, int resize_right, int resize_left, int resize_down, int resize_up){
    Field::x_size += resize_right + resize_left;
    Field::y_size += resize_down + resize_up;
    int** new_field = new int*[Field::y_size];
    for(int16_t i = 0; i < Field::y_size; ++i) {
        new_field[i] = new int[Field::x_size];
    }
    if (resize_up == 1) {
        for(int16_t k = 0; k < Field::x_size; ++k) {
            new_field[0][k] = 0;
        }
    }
    if (resize_down == 1) {
        for(int16_t k = 0; k < Field::x_size; ++k){
            new_field[Field::y_size-1][k] = 0;
        }
    }
    if (resize_left == 1) {
        for(int16_t k = 0; k < Field::y_size; ++k){
            new_field[k][0] = 0;
        }
    }
    if (resize_right == 1) {
        for(int16_t k = 0; k < Field::y_size; ++k){
            new_field[k][Field::x_size-1] = 0;
        }
    }
    for(int16_t i = resize_up; i < Field::y_size-resize_down; ++i) {
        for(int16_t j = resize_left; j < Field::x_size-resize_right; ++j){
            new_field[i][j] = field[i-resize_up][j-resize_left];
        }
        delete[] field[i-resize_up];
    }
    delete[] field;
    return new_field;
}

void CheckForResize(int& resize_right, int& resize_left, int& resize_down, int& resize_up){
    for(int i = 0; i < Field::y_size; ++i){
        if(Field::field[i][0] >= 4){
            resize_left = 1;
            break;
        }
    }
    for(int i = 0; i < Field::y_size; ++i){
        if(Field::field[i][Field::x_size-1] >= 4){
            resize_right = 1;
            break;
        }
    }
    for(int i = 0; i < Field::x_size; ++i){
        if(Field::field[0][i] >= 4){
            resize_up = 1;
            break;
        }
    }
    for(int i = 0; i < Field::x_size; ++i){
        if(Field::field[Field::y_size-1][i] >= 4){
            resize_down = 1;
            break;
        }
    }
}

bool Collapse() {
    bool was_collapse = false;
    int** new_field = new int*[Field::y_size];
    for(int16_t i = 0; i < Field::y_size; ++i) {
        new_field[i] = new int[Field::x_size];
        for(int16_t j = 0; j < Field::x_size; ++j){
            new_field[i][j] = 0;
        }
    }
    int16_t y_coord_fixed = Field::y_size;
    int16_t x_coord_fixed = Field::x_size;
    int resize_up = 0;
    int resize_down = 0;
    int resize_left = 0;
    int resize_right = 0;
    CheckForResize(resize_right, resize_left, resize_down, resize_up);
    new_field = ResizeField(new_field, resize_right, resize_left, resize_down, resize_up);
    for(int16_t i = 0; i < y_coord_fixed; ++i){
        for(int16_t j = 0; j < x_coord_fixed; ++j){
            if(Field::field[i][j] >= 4){
                was_collapse = true;
                new_field[i - 1 + resize_up][j + resize_left] += Field::field[i][j] / 4;
                new_field[i + resize_up][j - 1 + resize_left] += Field::field[i][j] / 4;
                new_field[i+1 + resize_up][j + resize_left] += Field::field[i][j] / 4;
                new_field[i + resize_up][j + 1 + resize_left] += Field::field[i][j] / 4;
            }
            new_field[i+resize_up][j+resize_left] += Field::field[i][j] % 4;
        }
    }
    for(int i = 0; i < y_coord_fixed; ++i){
        delete[] Field::field[i];
    }
    delete[] Field::field;
    Field::field = new int*[Field::y_size];
    for(int i = 0; i < Field::y_size; ++i){
        Field::field[i] = new int[Field::x_size];
        for(int j = 0; j < Field::x_size; ++j){
            Field::field[i][j] = new_field[i][j];
        }
    }
    return was_collapse;
}

void SandPile() {
    std::ifstream file;
    if (OpenFile(file)) {
        FillFieldOnStart(file);
        PrintField(Field::field);
        int count = 0;
        while(count != Arguments::max_iter && Collapse()){
            ++count;
            PrintField(Field::field);
        }
    }
}