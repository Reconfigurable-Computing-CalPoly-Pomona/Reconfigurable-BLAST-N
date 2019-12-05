#pragma once

#include "../util/types.hpp"

namespace Blastn {

/**
 * @brief Split an input string into words of a given length
 */
vector<string> split_to_words(string& input, u32 word_len);

} // Blastn