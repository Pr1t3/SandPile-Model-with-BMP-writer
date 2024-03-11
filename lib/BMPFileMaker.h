#pragma once

#include "Structures.hpp"

#include <charconv>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <cmath>
#include <filesystem>
#include <string>

void saveBMP(std::string& filename, Field& main_field);

std::string makeFileName(int64_t count_of_pictures);
