#include <algorithm>

#include "pairs.hpp"
#include "../util/display.hpp"

namespace Blastn {

#define ABS(value) ((s32)(value) < 0 ? (value) * -1 : (value))

vector<AdjacentPair> pair(vector<MatchSingleton>& flattened, u32 query_len)
{
    vector<AdjacentPair> result;

    for (u32 i = 0; i < flattened.size(); i++) {
        for (u32 j = i + 1; j < flattened.size(); j++) {
            // not overlapping
            if (ABS(((s32)flattened[i].sindex - (s32)flattened[j].sindex)) >= (s32)flattened[i].word.size()
                // not too far apart
                && ABS(((s32)flattened[i].sindex - (s32)flattened[j].sindex)) <= (s32)query_len - (s32)flattened[i].word.size())
            {
                result.emplace_back(
                    flattened[i].word,   flattened[j].word,
                    flattened[i].sindex, flattened[i].qindex,
                    flattened[j].sindex, flattened[j].qindex
                );

                break;
            }
        }
    }
    return result;
}

vector<AdjacentPair> flatten(vector<Match>& matches, u32 query_len)
{
    vector<MatchSingleton> flattened;

    for (auto& match : matches) {
        for (auto& sindex : match.subject_indices) {
            for (auto& qindex : match.query_indices) {
                flattened.push_back(MatchSingleton {
                    match.word, sindex, qindex
                });
            }
        }
    }
    std::sort(flattened.begin(), flattened.end(), [&](const MatchSingleton lhs, const MatchSingleton rhs) {
        return lhs.sindex < rhs.sindex;
    });

    return pair(flattened, query_len);
}

PairedSequenceMap pair_filter(MatchedSequenceMap& matches, SequenceMap& query)
{
    PairedSequenceMap filtered_pairs;
    Progress progress{ matches.size() };

    bool found = false;

    for (auto& sname_queries : matches) {
        PairedMatchesMap pairs;
        for (auto& qname_matches : sname_queries.second) {
            // qname not created yet in pairs
            for (auto& pair : flatten(qname_matches.second, (u32)query[qname_matches.first].size())) {
                if (ABS((s32)pair.sindex1 - (s32)pair.sindex2) <= (s32)query[qname_matches.first].size() - (s32)pair.length
                    || ABS((s32)pair.qindex1 - (s32)pair.qindex2) >= (s32)pair.length
                    || ABS((s32)pair.sindex1 - (s32)pair.sindex2) >= (s32)pair.length)
                {
                    // check if the pair was recorded yet
                    found = false;
                    for (auto& p : pairs[qname_matches.first]) {
                        if (pair.sindex1 == p.sindex1) {
                            found = true;
                            break;
                        }
                    }
                    // vector<Pair> already exists
                    if (!found)
                        pairs[qname_matches.first].emplace_back(pair);
                }
            }
        }
        if (!pairs.empty())
            filtered_pairs[sname_queries.first] = pairs;

        progress.update();
    }
    return filtered_pairs;
}

} // Blastn