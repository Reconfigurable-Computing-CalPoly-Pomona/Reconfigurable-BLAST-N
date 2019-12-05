#pragma once

#include "../util/types.hpp"

namespace Blastn {

/**
 * @brief The Dust algorithm for scoring words based on sub patterns of self-similarity.
 * @param word The word to score based on its self-similarity.
 * @param pattern_len The lengths of sections in word to see how self similar they are.
 * @return The Dust score.
 */
f32 dust(string word, u32 pattern_len);

/**
 * @brief Filter the IndexedSequenceMap using the dust algorithm.
 * @param query The format of the query to be filtered.
 * @param threshold Dust scores less than this threshold will be removed from query.
 * @param pattern_len The length of the patterns to look for in each word.
 * @return A copy of the IndexedSequenceMap with low scoring words removed.
 */
IndexedSequenceMap dust_filter(IndexedSequenceMap& query, f32 threshold, u32 pattern_len);

} // Blastn