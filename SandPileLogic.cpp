#include "SandPileLogic.h"

#pragma pack(push, 1)

struct BMPHeader {
    char signature[2]{'B', 'M'};
    uint32_t fileSize;
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t dataOffset;
    uint32_t headerSize{40};
    uint32_t width;
    uint32_t height;
    uint16_t planes{1};
    uint16_t bitsPerPixel{4};
    uint32_t compression{0};
    uint32_t dataSize{0};
    uint32_t horizontalResolution{0};
    uint32_t verticalResolution{0};
    uint32_t colors{16};
    uint32_t importantColors{5};
};

#pragma pack(pop)

void saveBMP(std::ofstream& file) {
    const int width = Field::x_size;
    const int height = Field::y_size;
    int width_in_byte = ceil(static_cast<double>(width)/2);
    int padding = (4 - width_in_byte%4)%4;

    BMPHeader header;
    header.fileSize = 74 + (width_in_byte+padding)*height;
    header.dataOffset = 74;
    header.width = width+padding;
    header.height = height;
    header.dataSize = (width_in_byte+padding)*height;

    file.write(reinterpret_cast<char*>(&header), 54);

    uint8_t colorPalette[20] = {
            0xFF, 0xFF, 0xFF, 0x00, // White
            0x00, 0xFF, 0x00, 0x00, // Green
            0x00, 0xFF, 0xFF, 0x00, // Yellow
            0x80, 0x00, 0x80, 0x00, // Purple
            0x00, 0x00, 0x00, 0x00, // Black
    };

    file.write(reinterpret_cast<char*>(colorPalette), 20);
    uint8_t* row = new uint8_t[width_in_byte+padding];
    for(int i = height-1; i >= 0; --i){
        for(int j = 0; j < width_in_byte+padding; ++j){
            row[j] = 0x00;
        }
        uint8_t pixels = 0x00;
        int count = 0;
        for(int j = 0; j < width; ++j) {
            switch (Field::field[i][j]) {
                case 0:
                    pixels |= 0x00;
                    break;
                case 1:
                    pixels |= 0x01;
                    break;
                case 2:
                    pixels |= 0x02;
                    break;
                case 3:
                    pixels |= 0x03;
                    break;
                default:
                    pixels |= 0x04;
                    break;
            }
            ++count;
            if (count % 2 == 0) {
                row[count/2 - 1] = pixels;
                pixels = 0x00;
            } else {
                pixels <<= 4;
            }
        }
        if(pixels != 0x00){
            row[(count+2)/2 - 1] = pixels |= 0x00;
        }
        file.write(reinterpret_cast<char*>(row), width_in_byte+padding);
    }
}

bool OpenFile(std::ifstream& file) {
    file.open(Arguments::input_file);
    if (!file.is_open()) {
        std::cerr << "File not found";
        return false;
    }
    return true;
}

char* ConvertFromIntToChars(int64_t num, char* chars_num){
    for(int64_t i = static_cast<int>(ceil(log10(num+1)))-1; i >= 0; --i){
        chars_num[i] = static_cast<char>(num % 10)+'0';
        num /= 10;
    }
    return chars_num;
}

char* makeFileName(int64_t count_of_pictures){
    char* name = new char[static_cast<int>(ceil(log10(count_of_pictures+1)))+1];
    name = ConvertFromIntToChars(count_of_pictures, name);
    name[static_cast<int>(ceil(log10(count_of_pictures+1)))] = '\0';
    char* filename = new char[SizeOfString(Arguments::output_folder) + SizeOfString(name)+6];
    for(int64_t i = 0; i < SizeOfString(Arguments::output_folder) + SizeOfString(name)+1; ++i){
        if(i < SizeOfString(Arguments::output_folder)){
            filename[i] = Arguments::output_folder[i];
        } else if(i == SizeOfString(Arguments::output_folder)) {
            filename[i] = '\\';
        } else {
            filename[i] = name[i-SizeOfString(Arguments::output_folder)-1];
        }
    }
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name)+1] = '.';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name)+2] = 'b';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name)+3] = 'm';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name)+4] = 'p';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name)+5] = '\0';
    delete[] name;
    return filename;
}

void FillFieldOnStart(std::ifstream& file) {
    bool is_first = true;
    bool is_second = false;
    int num = 0;
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
        if(isdigit(symbol)) {
            int n = static_cast<int>(symbol - '0');
            if (is_first) {
                num *= 10;
                num += n;
            } else if (is_second) {
                num *= 10;
                num += n;
            }
        } else {
            if(is_first){
                is_first = false;
                is_second = true;
                if (Field::x_size < num) {
                    Field::x_size = num;
                }
                num = 0;
            } else if (is_second){
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
        for(int j = 0; j < Field::x_size; ++j){
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
        if(isdigit(symbol)) {
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
            if(is_first){
                is_first = false;
                is_second = true;
            } else if (is_second){
                is_second = false;
            }
        }
    }
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
        for(int j = 0; j < Field::x_size; ++j){
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
    Field::field = new_field;
    return was_collapse;
}

void PrintFile(){
    for(int i = 0; i < Field::y_size; ++i){
        for(int j = 0; j < Field::x_size; ++j){
            std::cout << Field::field[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

void SandPile() {
    std::ifstream input_file;
    if (OpenFile(input_file)) {
        FillFieldOnStart(input_file);
        int count_of_pictures = 1;
        char* filename = makeFileName(count_of_pictures);
        std::ofstream file(filename, std::ios::binary);
        saveBMP(file);
        file.close();
        delete[] filename;
        int count_of_iterations = 0;
        while(count_of_iterations != Arguments::max_iter && Collapse()) {
            ++count_of_iterations;
            if(count_of_iterations % Arguments::freq == 0){
                ++count_of_pictures;
                char* filename = makeFileName(count_of_pictures);
                std::ofstream file(filename, std::ios::binary);
                saveBMP(file);
                file.close();
                delete[] filename;
            }
        }
        ++count_of_pictures;
        filename = makeFileName(count_of_pictures);
        file.open(filename, std::ios::binary);
        saveBMP(file);
        file.close();
        delete[] filename;
    }
}