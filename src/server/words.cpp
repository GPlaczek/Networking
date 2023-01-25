#include "words.hpp"

#include <vector>
#include <fstream>
#include <time.h>
#include <stdio.h>

std::vector<std::string> Words::words = {};

void Words::load_words(const char *path) {
    std::ifstream f1;
    f1.open(path);

    if (f1.fail()) {
        fprintf(stderr, "Could not load word list\n");
        exit(1);
    }

    std::string line;
    while (std::getline(f1, line)) { Words::words.push_back(std::move(line)); }
    f1.close();
}

std::string Words::get_word() {
    unsigned long j;
    srand((unsigned)time(NULL));
    int index = rand() % words.size();
    return Words::words[index];
}
