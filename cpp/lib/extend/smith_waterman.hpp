#pragma once

#include "../../util/types.hpp"

namespace Blastn {

/**
 * Holds the single greatest element, whether it be
 * from an up, left, or diagonal score
 */
struct Greatest {
    u32 index;
    s32 value;
};

/**
 * @brief The smith waterman algorithm for aligning two sequences.
 * @param seq1 The first sequence to align.
 * @param seq2 The second sequence to align.
 * @param match The score when two characters are the same.
 * @param mismatch The score when two characters are not the same.
 * @param gap The score when there is a gap character.
 * @param just_score Indicate to exit the function early to only calculate the score.
 * @return The smith waterman score.
 */
s32 smith_waterman(string& seq1, string& seq2, s32 match, s32 mismatch, s32 gap, bool just_score);

/**
 * @brief The smith waterman algorithm, but mallocs and frees the score matrix each time
 */
s32 smith_waterman_no_preserve(string& seq1, string& seq2, s32 match, s32 mismatch, s32 gap);

/**
 * @brief The smith waterman algorithm, but mallocs the first time, and preserves the score matrix
 *        into a static variable. The score matrix will grow as needed, and is reclaimed when the program exits.
 */
s32 smith_waterman_preserve(const char *seq1, const char *seq2, s32 match, s32 mismatch, s32 gap, u64 cols, u64 rows);

/**
 * @brief The smith waterman algorithm, but creates teams of threads using OpenMP to calculate
 *        the up, left, and diagonal scores simultaneously. This is fast by itself, but the cost
 *        of creating and destroying threads very often is too expensive to use with the 'extend' algorithm
 */
s32 smith_waterman_mt(string& seq1, string& seq2, s32 match, s32 mismatch, s32 gap);

/**
 * @brief The smith waterman algorithm interface to communicate with the FPGA. It sends
 *        the data to score, and receives a score back
 */
s32 smith_waterman_fgpa(const char *seq1, const char *seq2, u64 size);

} // Blastn