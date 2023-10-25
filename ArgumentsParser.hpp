#pragma once

#include "Structures.hpp"
#include "utilities.h"

#include <iostream>
#include <cstring>
#include <charconv>
#include <cmath>

bool ParseArguments(int argc, char** argv, char** possible_arguments, int& cnt_of_possible_arguments);

bool MakeArguments(char** possible_arguments, int size);