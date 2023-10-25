#include "utilities.h"

char* ConvertFromIntToChars(int64_t num, char* chars_num) {
    for (int64_t i = static_cast<int>(ceil(log10(num + 1))) - 1; i >= 0; --i) {
        chars_num[i] = static_cast<char>(num % 10) + '0';
        num /= 10;
    }
    return chars_num;
}

int SizeOfString(const char* string) {
    int index = 0;
    while (string[index] != '\0' &&
           ((static_cast<int>(string[index]) >= 33 && static_cast<int>(string[index]) <= 126) ||
            static_cast<int>(string[index]) < 0)) {
        ++index;
    }
    return index;
}

bool ConvertFromCharsToInt(int size, const char* string, uint64_t& num) {
    int result = 0;
    for (int i = 0; i < size; ++i) {
        if (std::isdigit(string[i])) {
            result += pow(10, size - i - 1) * (string[i] - '0');
        } else {
            return false;
        }
    }
    num = result;
    return true;
}

bool IsFileOpened(std::ifstream& file) {
    file.open(Arguments::input_file);
    if (!file.is_open()) {
        std::cerr << "File not found";
        return false;
    }
    return true;
}