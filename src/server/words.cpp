#include "words.hpp"

#include <vector>
#include <fstream>
#include <time.h>

std::vector<std::string> Words::words = {};

void Words::load_words(const char *path) {
    std::ifstream f1;
    f1.open(path);
    std::string line;
    while (std::getline(f1, line)) { Words::words.push_back(std::move(line)); }
}

std::string Words::get_word() {
    unsigned long j;
    srand((unsigned)time(NULL));
    int index = rand() % words.size();
    return Words::words[index];
}
