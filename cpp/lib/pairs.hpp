#pragma once

#include "../util/types.hpp"

namespace Blastn {

/**
 * A flattened match. Normally, Match has a vector of s and q indices. Use
 * to store an individual slice of a Match
 */
struct MatchSingleton {
    string word;
    u32 sindex;
    u32 qindex;
};

/**
 * @brief Combine MatchSingletons together into a pair if they are adjacent and close enough
 * @param flattened The vector of MatchSingletons converted from the vector of Matches
 * @param query_len Length of the query to determine if a possible pair are close enough together
 * @return The collection of adjacent pairs made from match singletons
 */
vector<AdjacentPair> pair(vector<MatchSingleton>& flattened, u32 query_len);

/**
 * @brief Turn matches into a vector of match singletons, then pair them
 * @param matches   The vector of matches to flatten
 * @param query_len Query length to pass to pair
 * @return The collection of adjacent pairs made from match singletons
 */
vector<AdjacentPair> flatten(vector<Match>& matches, u32 query_len);

/**
 * @brief Go through all matches and convert each query's Matches into AdjacentPairs
 * @param matches The match sequence map to find pairs within
 * @param query   The query sequence map to pass the length of to helper functions
 * @return The sequence map of adjacent pairs
 */
PairedSequenceMap pair_filter(MatchedSequenceMap& matches, SequenceMap& query);

} // Blastn