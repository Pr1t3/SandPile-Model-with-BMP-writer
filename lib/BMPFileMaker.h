#pragma once

#include "Structures.hpp"

#include <charconv>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <cmath>

void saveBMP(char* filename, Field& main_field);

char* makeFileName(int64_t count_of_pictures);