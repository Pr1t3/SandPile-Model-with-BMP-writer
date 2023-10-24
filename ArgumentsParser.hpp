#pragma once

#include <iostream>
#include <cstring>
#include <charconv>
#include <cmath>

#include "Structures.hpp"

int SizeOfString(const char* string);

bool ParseArguments(int argc, char** argv, char** possible_arguments, int& cnt_of_possible_arguments);

bool MakeArguments(char** possible_arguments, int size);