#include "ArgumentsParser.hpp"

int main(int argc, char** argv) {
    Arguments arguments;
    Flags flags;
    int max_cnt_symbols = 0;
    for(int i = 1; i < argc; ++i){
        int cnt_symbols = 0;
        int j = 0;
        while(argv[i][j] != '\0'){
            ++cnt_symbols;
            ++j;
        }
        if(cnt_symbols > max_cnt_symbols){
            max_cnt_symbols = cnt_symbols;
        }
    }
    char** possible_arguments = new char*[argc-1];
    for(int i = 0; i < argc-1; ++i){
        possible_arguments[i] = new char[max_cnt_symbols];
    }
    bool is_ok;
    int cnt_of_possible_arguments = 0;
    is_ok = ParseArguments(argc, argv, possible_arguments, cnt_of_possible_arguments);
    if (is_ok) {
        is_ok = MakeArguments(possible_arguments, cnt_of_possible_arguments, arguments, flags);
    }
    for(int i = 0; i < cnt_of_possible_arguments; ++i){
        delete[] possible_arguments[i];
    }
    delete[] possible_arguments;
}