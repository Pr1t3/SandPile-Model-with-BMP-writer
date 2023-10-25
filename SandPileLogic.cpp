#include "SandPileLogic.h"

void FillFieldOnStart(std::ifstream& file) {
    bool is_first = true;
    bool is_second = false;
    int num = 0;
    file.clear();
    file.seekg(0);
    while (file) {
        char symbol = file.get();
        if (symbol == EOF) {
            break;
        }
        if (symbol == '\r' || symbol == '\n') {
            is_first = true;
            is_second = true;
            num = 0;
            continue;
        }
        if (isdigit(symbol)) {
            int digit = static_cast<int>(symbol - '0');
            if (is_first) {
                num *= 10;
                num += digit;
            } else if (is_second) {
                num *= 10;
                num += digit;
            }
        } else {
            if (is_first) {
                is_first = false;
                is_second = true;
                if (Field::x_size < num) {
                    Field::x_size = num;
                }
                num = 0;
            } else if (is_second) {
                is_second = false;
                if (Field::y_size < num) {
                    Field::y_size = num;
                }
            }
        }
    }
    ++Field::y_size;
    ++Field::x_size;
    Field::field = new int* [Field::y_size];
    for (int i = 0; i < Field::y_size; ++i) {
        Field::field[i] = new int[Field::x_size];
        for (int j = 0; j < Field::x_size; ++j) {
            Field::field[i][j] = 0;
        }
    }
    file.clear();
    file.seekg(0);
    int x = 0;
    int y = 0;
    is_first = true;
    is_second = false;
    while (file) {
        char symbol = file.get();
        if (symbol == EOF) {
            break;
        }
        if (symbol == '\r' || symbol == '\n') {
            x = 0;
            y = 0;
            is_first = true;
            is_second = false;
            continue;
        }
        if (isdigit(symbol)) {
            int num = static_cast<int>(symbol - '0');
            if (is_first) {
                x *= 10;
                x += num;
            } else if (is_second) {
                y *= 10;
                y += num;
            } else {
                Field::field[y][x] *= 10;
                Field::field[y][x] += num;
            }
        } else {
            if (is_first) {
                is_first = false;
                is_second = true;
            } else if (is_second) {
                is_second = false;
            }
        }
    }
}

int** ResizeField(int** field, int resize_right, int resize_left, int resize_down, int resize_up) {
    Field::x_size += resize_right + resize_left;
    Field::y_size += resize_down + resize_up;
    int** new_field = new int* [Field::y_size];
    for (int16_t i = 0; i < Field::y_size; ++i) {
        new_field[i] = new int[Field::x_size];
    }
    if (resize_up == 1) {
        for (int16_t k = 0; k < Field::x_size; ++k) {
            new_field[0][k] = 0;
        }
    }
    if (resize_down == 1) {
        for (int16_t k = 0; k < Field::x_size; ++k) {
            new_field[Field::y_size - 1][k] = 0;
        }
    }
    if (resize_left == 1) {
        for (int16_t k = 0; k < Field::y_size; ++k) {
            new_field[k][0] = 0;
        }
    }
    if (resize_right == 1) {
        for (int16_t k = 0; k < Field::y_size; ++k) {
            new_field[k][Field::x_size - 1] = 0;
        }
    }
    for (int16_t i = resize_up; i < Field::y_size - resize_down; ++i) {
        for (int16_t j = resize_left; j < Field::x_size - resize_right; ++j) {
            new_field[i][j] = field[i - resize_up][j - resize_left];
        }
        delete[] field[i - resize_up];
    }
    delete[] field;
    return new_field;
}

void CheckForResize(int& resize_right, int& resize_left, int& resize_down, int& resize_up) {
    for (int i = 0; i < Field::y_size; ++i) {
        if (Field::field[i][0] >= 4) {
            resize_left = 1;
            break;
        }
    }
    for (int i = 0; i < Field::y_size; ++i) {
        if (Field::field[i][Field::x_size - 1] >= 4) {
            resize_right = 1;
            break;
        }
    }
    for (int i = 0; i < Field::x_size; ++i) {
        if (Field::field[0][i] >= 4) {
            resize_up = 1;
            break;
        }
    }
    for (int i = 0; i < Field::x_size; ++i) {
        if (Field::field[Field::y_size - 1][i] >= 4) {
            resize_down = 1;
            break;
        }
    }
}

void Collapse(bool& was_collapsed) {
    was_collapsed = false;
    int** new_field = new int* [Field::y_size];
    for (int16_t i = 0; i < Field::y_size; ++i) {
        new_field[i] = new int[Field::x_size];
        for (int j = 0; j < Field::x_size; ++j) {
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
    for (int16_t i = 0; i < y_coord_fixed; ++i) {
        for (int16_t j = 0; j < x_coord_fixed; ++j) {
            if (Field::field[i][j] >= 4) {
                was_collapsed = true;
                new_field[i - 1 + resize_up][j + resize_left] += Field::field[i][j] / 4;
                new_field[i + resize_up][j - 1 + resize_left] += Field::field[i][j] / 4;
                new_field[i + 1 + resize_up][j + resize_left] += Field::field[i][j] / 4;
                new_field[i + resize_up][j + 1 + resize_left] += Field::field[i][j] / 4;
            }
            new_field[i + resize_up][j + resize_left] += Field::field[i][j] % 4;
        }
    }
    for (int i = 0; i < y_coord_fixed; ++i) {
        delete[] Field::field[i];
    }
    delete[] Field::field;
    Field::field = new_field;
}

void SandPile(std::ifstream& input_file) {
    FillFieldOnStart(input_file);
    int count_of_pictures = 1;
    char* filename = makeFileName(count_of_pictures);
    saveBMP(filename);
    int count_of_iterations = 0;
    bool was_collapsed = false;
    while (count_of_iterations != Arguments::max_iter && was_collapsed || count_of_iterations == 0) {
        Collapse(was_collapsed);
        ++count_of_iterations;
        if (count_of_iterations % Arguments::freq == 0) {
            ++count_of_pictures;
            char* filename = makeFileName(count_of_pictures);
            saveBMP(filename);
        }
    }
    if(!(count_of_iterations == 1 && !was_collapsed) && count_of_iterations % Arguments::freq != 0) {
        ++count_of_pictures;
        filename = makeFileName(count_of_pictures);
        saveBMP(filename);
    }
}