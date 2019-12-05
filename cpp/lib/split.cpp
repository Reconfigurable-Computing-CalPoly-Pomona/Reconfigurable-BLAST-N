#include "split.hpp"

namespace Blastn {

vector<string> split_to_words(string& input, u32 word_len) {
    u32 num_words = (u32)input.length() - word_len + 1;
    vector<string> words;
    words.reserve(num_words);

    // get all substrings of the same length in the input
    for (u32 i = 0; i < num_words; i++)
        words.emplace_back(input.substr(i, word_len));

    return words;
}

} // Blastn