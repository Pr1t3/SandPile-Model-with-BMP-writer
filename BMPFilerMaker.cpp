#include "BMPFileMaker.h"

char* makeFileName(int64_t count_of_pictures) {
    char* name = new char[static_cast<int>(ceil(log10(count_of_pictures + 1))) + 1];
    name = ConvertFromIntToChars(count_of_pictures, name);
    name[static_cast<int>(ceil(log10(count_of_pictures + 1)))] = '\0';
    char* filename = new char[SizeOfString(Arguments::output_folder) + SizeOfString(name) + 6];
    for (int64_t i = 0; i < SizeOfString(Arguments::output_folder) + SizeOfString(name) + 1; ++i) {
        if (i < SizeOfString(Arguments::output_folder)) {
            filename[i] = Arguments::output_folder[i];
        } else if (i == SizeOfString(Arguments::output_folder)) {
            filename[i] = '\\';
        } else {
            filename[i] = name[i - SizeOfString(Arguments::output_folder) - 1];
        }
    }
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name) + 1] = '.';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name) + 2] = 'b';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name) + 3] = 'm';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name) + 4] = 'p';
    filename[SizeOfString(Arguments::output_folder) + SizeOfString(name) + 5] = '\0';
    delete[] name;
    return filename;
}

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

void saveBMP(char* filename) {
    std::ofstream file(filename, std::ios::binary);
    const int width = Field::x_size;
    const int height = Field::y_size;
    int width_in_byte = ceil(static_cast<double>(width) / 2);
    int padding = (4 - width_in_byte % 4) % 4;

    BMPHeader header;
    header.fileSize = 74 + (width_in_byte + padding) * height;
    header.dataOffset = 74;
    header.width = width + padding;
    header.height = height;
    header.dataSize = (width_in_byte + padding) * height;

    file.write(reinterpret_cast<char*>(&header), 54);

    uint8_t colorPalette[20] = {
            0xFF, 0xFF, 0xFF, 0x00, // White
            0x00, 0xFF, 0x00, 0x00, // Green
            0x00, 0xFF, 0xFF, 0x00, // Yellow
            0x80, 0x00, 0x80, 0x00, // Purple
            0x00, 0x00, 0x00, 0x00, // Black
    };

    file.write(reinterpret_cast<char*>(colorPalette), 20);
    uint8_t* row = new uint8_t[width_in_byte + padding];
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width_in_byte + padding; ++j) {
            row[j] = 0x00;
        }
        uint8_t pixels = 0x00;
        int count = 0;
        for (int j = 0; j < width; ++j) {
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
                row[count / 2 - 1] = pixels;
                pixels = 0x00;
            } else {
                pixels <<= 4;
            }
        }
        if (pixels != 0x00) {
            row[(count + 2) / 2 - 1] = pixels;// |= 0x00;
        }
        file.write(reinterpret_cast<char*>(row), width_in_byte + padding);
    }
    delete[] filename;
    file.close();
}