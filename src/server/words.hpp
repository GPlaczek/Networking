#include <vector>
#include <string>

class Words {
    static std::vector<std::string> words;
public:
    static void load_words(const char *path);
    static std::string get_word();
};
