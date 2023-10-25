#pragma once

#include "Structures.hpp"

#include <cstdint>
#include <cmath>
#include <iostream>
#include <fstream>

char* ConvertFromIntToChars(int64_t num, char* chars_num);

int SizeOfString(const char* string);

bool ConvertFromCharsToInt(int size, const char* string, uint64_t& lines);

bool IsFileOpened(std::ifstream& file);