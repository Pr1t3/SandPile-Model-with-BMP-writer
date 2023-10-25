#include "ArgumentsParser.hpp"

bool ParseArguments(int argc, char** argv, char** possible_arguments, int& cnt_of_possible_arguments) {
    char* temp = new char[0];
    char** temp_argv = new char* [argc];
    for (int i = 1; i < argc; ++i) {
        temp_argv[i - 1] = new char[SizeOfString(argv[i]) + 1];
        for (int j = 0; j < SizeOfString(argv[i]); ++j) {
            temp_argv[i - 1][j] = argv[i][j];
        }
        temp_argv[i - 1][SizeOfString(argv[i])] = '\0';
    }
    int index = 0;
    for (int i = 0; i < argc - 1; ++i) {
        if (temp_argv[i][0] == '-' && SizeOfString(temp) == 0) {
            memset(temp, 0, SizeOfString(temp));
            delete[] temp;
            temp = new char[SizeOfString(temp_argv[i]) + 1];
            memset(temp, 0, SizeOfString(temp));
            for (int j = 0; j < SizeOfString(temp_argv[i]); ++j) {
                temp[j] = temp_argv[i][j];
            }
            temp[SizeOfString(temp_argv[i])] = '\0';
            for (int j = 0; j < SizeOfString(temp_argv[i]); ++j) {
                if (temp_argv[i][j] == '=') {
                    for (int k = 0; k < SizeOfString(temp); ++k) {
                        possible_arguments[index][k] = temp[k];
                    }
                    possible_arguments[index][SizeOfString(temp) + 1] = '\0';
                    ++index;
                    ++cnt_of_possible_arguments;
                    memset(temp, 0, SizeOfString(temp));
                    delete[] temp;
                    if (i != argc - 1) {
                        temp = new char[1];
                        memset(temp, 0, SizeOfString(temp));
                    }
                    break;
                }
            }
        } else if (temp_argv[i][0] == '-' && SizeOfString(temp) != 0) {
            for (int k = 0; k < SizeOfString(temp); ++k) {
                possible_arguments[index][k] = temp[k];
            }
            possible_arguments[index][SizeOfString(temp) + 1] = '\0';
            ++index;
            ++cnt_of_possible_arguments;
            memset(temp, 0, SizeOfString(temp));
            delete[] temp;
            if (i != argc - 1) {
                temp = new char[1];
                memset(temp, 0, SizeOfString(temp));
            }
            for (int k = 0; k < SizeOfString(temp); ++k) {
                possible_arguments[index][k] = temp_argv[i][k];
            }
            possible_arguments[index][SizeOfString(temp) + 1] = '\0';
        } else if (temp_argv[i][0] != '-' && SizeOfString(temp) != 0) {
            char* temp_temp = new char[SizeOfString(temp)];
            memset(temp_temp, 0, SizeOfString(temp_temp));
            for (int j = 0; j < SizeOfString(temp); ++j) {
                temp_temp[j] = temp[j];
            }
            memset(temp, 0, SizeOfString(temp));
            delete[] temp;
            temp = new char[SizeOfString(temp_temp) + SizeOfString(temp_argv[i]) + 1];
            memset(temp, 0, SizeOfString(temp));
            for (int j = 0; j < SizeOfString(temp_temp) + SizeOfString(temp_argv[i]) + 1; ++j) {
                if (j < SizeOfString(temp_temp)) {
                    temp[j] = temp_temp[j];
                } else if (j == SizeOfString(temp_temp)) {
                    temp[j] = '=';
                } else {
                    temp[j] = temp_argv[i][j - SizeOfString(temp_temp) - 1];
                }
            }
            for (int k = 0; k < SizeOfString(temp); ++k) {
                possible_arguments[index][k] = temp[k];
            }
            possible_arguments[index][SizeOfString(temp) + 1] = '\0';
            ++index;
            ++cnt_of_possible_arguments;
            memset(temp, 0, SizeOfString(temp));
            delete[] temp;
            if (i != argc - 1) {
                temp = new char[1];
                memset(temp, 0, SizeOfString(temp));
            }
            delete[] temp_temp;
        } else {
            std::cerr << "Wrong Input";
            return false;
        }
        if (i == argc - 2 && SizeOfString(temp) != 0) {
            for (int k = 0; k < SizeOfString(temp); ++k) {
                possible_arguments[index][k] = temp[k];
            }
            possible_arguments[index][SizeOfString(temp) + 1] = '\0';
            ++index;
            ++cnt_of_possible_arguments;
            delete[] temp;
        }
    }
    for (int i = 0; i < argc - 1; ++i) {
        delete[] temp_argv[i];
    }
    delete[] temp_argv;
    return true;
}

bool MakeArguments(char** possible_arguments, int size) {
    for (int i = 0; i < size; ++i) {
        bool is_it_name = true;
        int size_of_temp_name = 0;
        int size_of_temp_value = 0;
        for (int j = 0; j < SizeOfString(possible_arguments[i]); ++j) {
            if (possible_arguments[i][j] == '=') {
                is_it_name = false;
            } else if (is_it_name) {
                ++size_of_temp_name;
            } else {
                ++size_of_temp_value;
            }
        }
        is_it_name = true;
        char* temp_name = new char[size_of_temp_name + 1];

        char* temp_value = new char[size_of_temp_value + 1];

        for (int j = 0; j < SizeOfString(possible_arguments[i]); ++j) {
            if (possible_arguments[i][j] == '=') {
                is_it_name = false;
            } else if (is_it_name) {
                temp_name[j] = possible_arguments[i][j];
            } else {
                temp_value[j - size_of_temp_name - 1] = possible_arguments[i][j];
            }
        }
        temp_name[size_of_temp_name] = '\0';
        temp_value[size_of_temp_value] = '\0';
        if (strcmp(temp_name, "-i") == 0 || strcmp(temp_name, "--input") == 0) {
            if (!Flags::is_input_file_alone) {
                std::cerr << "Wrong Input. There is more than 1 argument(-o, --output)";
                delete[]  temp_name;
                delete[]  temp_value;
                return false;
            }
            delete[] Arguments::input_file;
            Arguments::input_file = new char[size_of_temp_value + 1];
            Arguments::input_file[size_of_temp_value] = '\0';
            for (int i = 0; i < size_of_temp_value; ++i) {
                Arguments::input_file[i] = temp_value[i];
            }
            Flags::is_input_file_alone = false;
        } else if (strcmp(temp_name, "-o") == 0 || strcmp(temp_name, "--output") == 0) {
            if (!Flags::is_output_file_alone) {
                std::cerr << "Wrong Input. There is more than 1 argument(-o, --output)";
                delete[]  temp_name;
                delete[]  temp_value;
                return false;
            }
            delete[] Arguments::output_folder;
            Arguments::output_folder = new char[size_of_temp_value + 1];
            Arguments::output_folder[size_of_temp_value] = '\0';
            for (int i = 0; i < size_of_temp_value; ++i) {
                Arguments::output_folder[i] = temp_value[i];
            }
            Flags::is_output_file_alone = false;
        } else if (strcmp(temp_name, "-m") == 0 || strcmp(temp_name, "--max-iter") == 0) {
            if (!Flags::is_max_iter_alone) {
                std::cerr << "Wrong Input. There is more than 1 argument(-m, --max-iter)";
                delete[]  temp_name;
                delete[]  temp_value;
                return false;
            }
            ConvertFromCharsToInt(size_of_temp_value, temp_value, Arguments::max_iter);
            Flags::is_max_iter_alone = false;
        } else if (strcmp(temp_name, "-f") == 0 || strcmp(temp_name, "--freq") == 0) {
            if (!Flags::is_freq_alone) {
                std::cerr << "Wrong Input. There is more than 1 argument(-f, --freq)";
                delete[]  temp_name;
                delete[]  temp_value;
                return false;
            }
            ConvertFromCharsToInt(size_of_temp_value, temp_value, Arguments::freq);
            Flags::is_freq_alone = false;
        } else {
            std::cerr << "Wrong Input. There is no such argument";
            delete[]  temp_name;
            delete[]  temp_value;
            return false;
        }
        delete[]  temp_name;
        delete[]  temp_value;
    }
    return true;
}