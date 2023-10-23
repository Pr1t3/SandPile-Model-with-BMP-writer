#pragma once

#include <iostream>
#include <string.h>
#include <charconv>
#include <math.h>

#include "Structures.hpp"

bool ParseArguments(int argc, char** argv, char** possible_arguments, int& cnt_of_possible_arguments);

bool MakeArguments(char** possible_arguments, int size);