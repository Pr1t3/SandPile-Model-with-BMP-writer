#pragma once

#include "utilities.h"
#include "Structures.hpp"

#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>

void saveBMP(char* filename);

char* makeFileName(int64_t count_of_pictures);