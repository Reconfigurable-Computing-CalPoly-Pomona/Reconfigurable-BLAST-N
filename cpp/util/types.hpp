#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <parallel_hashmap/phmap.h>

#include "constants.hpp"

namespace Blastn {

/**
 * Type definitions used throughout Blastn
 */

using byte = uint8_t;
using s32  = int32_t;
using u32  = uint32_t;
using u64  = uint64_t;
using f32  = float;
using f64  = double;

using string = std::string;

template<typename T, typename U>
using dict = phmap::flat_hash_map<T, U>;

template<typename T>
using vector = std::vector<T>;

/**
 * The details for what a match has, a word, subject indices, query indices.
 */
struct Match {
    Match(string word, vector<u32> subject_indices, vector<u32> query_indices);
    ~Match();

    string word;
    vector<u32> subject_indices;
    vector<u32> query_indices;
};

/**
 * After matching, save data about what each adjacent pair (within range) has for the query and subject
 */
struct AdjacentPair {
    AdjacentPair(string word1, string word2, u32 sindex1, u32 qindex1, u32 sindex2, u32 qindex2);
    ~AdjacentPair();

    string word1, word2;
    u32 length;
    u32 sindex1, sindex2;
    u32 qindex1, qindex2;
};

/**
 * After finding adjacent pairs, extend the adjacent pairs against the subject to create an extended pair
 */
struct Extended {
    Extended(string extended_pair, u32 sindex, u32 qindex, s32 score);
    ~Extended();

    string extended_pair;
    u32 sindex;
    u32 qindex;
    s32 score;
};

/**
 * Resultant objects from Blastn. Holds the information needed in the output file for displaying
 */
struct HSP {
    HSP(string subject_id, string query_id, string extended_pair, u32 sindex, u32 qindex, s32 sw_score);
    ~HSP();

    // calculated in constructor
    string subject_id;
    string query_id;
    string extended_pair;
    u32 subject_start, subject_end;
    u32 query_start, query_end;
    s32 sw_score;

    // calculate later

    f32 percentage_id = 0;
    u32 matches = 0;
    u32 mismatches = 0;
    u32 gaps = 0;

    f64 evalue = 0;
    f64 bitscore = 0;
};

/**
 * 
 * Renamed types used throughout Blastn.
 * Each type has a 'str' function associated with it for convenient printing and debugging
 * 
 */

/**
 * Used in Smith Waterman for the matrices
 */
using Matrix = vector<vector<u32>>;
string str(Matrix& m);
/**
 * Map sequence names to their sequence.
 */
using SequenceMap = dict<string, string>;
string str(SequenceMap& s);

/**
 * Intermediate, map a word to its indices.
 */
using IndexedWordMap = dict<string, std::vector<u32>>;
/**
 * Map sequence names to all words mapped to a vector or indices where each word appears in its sequence.
 */
using IndexedSequenceMap = dict<string, IndexedWordMap>;
string str(IndexedSequenceMap& s);

/**
 * Intermediate, map a query name to its Match objects
 */
using MatchedMatchesMap = dict<string, std::vector<Match>>;
/**
 * Subject name mapped to a query name mapped to a vector of Match objects
 */
using MatchedSequenceMap = dict<string, MatchedMatchesMap>;
string str(MatchedSequenceMap& s);

/**
 * Intermediate, map a query name to its AdjacentPair objects
 */
using PairedMatchesMap = dict<string, vector<AdjacentPair>>;
/**
 * Subject name mapped to a query name mapped to a vector of AdjacentPair objects 
 */
using PairedSequenceMap = dict<string, PairedMatchesMap>;
string str(PairedSequenceMap& s);

/**
 * Intermediate, map a query name to its Extended pair objects
 */
using ExtendedPairsMap = dict<string, std::vector<Extended>>;
/**
 * Subject name mapped to a query name mapped to a vector of Extended pair objects
 */
using ExtendedSequenceMap = dict<string, ExtendedPairsMap>;
string str(ExtendedSequenceMap& s);

/**
 * Output data structure
 */
string str(vector<HSP>& s);

} // Blastn