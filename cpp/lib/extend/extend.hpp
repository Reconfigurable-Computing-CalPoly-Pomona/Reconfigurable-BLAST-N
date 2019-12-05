#pragma once

#include "../../util/types.hpp"

namespace Blastn {

/**
 * @brief Extend a pair of words where each word is in the same subject and query. In addition,
 *        the distance between the words in the subject is less than the length of the query.
 *        This algorithm will extend the left word to the left and perform the Smith Waterman
 *        algorithm on it vs the subject equivlanent. Then the left word will be extended right
 *        until it either reaches the right word, or gaps need to be added. After, the right word
 *        is extended right and scored.
 * @param pair     The AdjacentPair with all of the information to know where each word is in their respective data structures.
 * @param query    The current query dataset to extend the pair from.
 * @param subject  The current subject to extend the pair to.
 * @param match    The smith waterman score for a match.
 * @param mismatch The smith waterman score for a mismatch.
 * @param gap      The smith waterman score for a gap.
 * @param minscore The minimum smith waterman score from the extended values. Pairs with scores below this are removed.
 * @param score    Indicate to score and remove words on true.
 * @param printing Indicate to print the values at the end of the function.
 * @return The extended, aligned match.
 */
Extended extend_and_score(AdjacentPair pair,
                          string& query,
                          string& subject,
                          s32 match,
                          s32 mismatch,
                          s32 gap,
                          f32 ratio,
                          bool score,
                          bool printing,
                          u32 flag);

/*
 * @brief: Given adjacent pairs, the subject and query, extend the pairs from the query to the subject.
 * @param pairs    The mapped subject of adjacent pairs to extend and filter while extending.
 * @param query    The map of query names to their entire query sequence.
 * @param subject  The map of subject names to their entire subject sequence.
 * @param minscore The minimum smith waterman score allowed before needing to remove the word.
 * @param match    The smith waterman score when two characters are the same.
 * @param mismatch The smith waterman score when two characters are not the same.
 * @param gap      The smith waterman score when there is a gap character.
 * @return A map of subject names to query names to a list of extended matches with their subject base index.
 */
ExtendedSequenceMap extend_filter(PairedSequenceMap& pairs,
                                  SequenceMap& query,
                                  SequenceMap& subject,
                                  s32 match,
                                  s32 mismatch,
                                  s32 gap,
                                  f32 ratio,
                                  int flag);

} // Blastn