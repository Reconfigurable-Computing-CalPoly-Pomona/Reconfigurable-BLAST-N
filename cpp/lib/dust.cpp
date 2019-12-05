#include <algorithm>    // count

#include "dust.hpp"
#include "split.hpp"    // split_to_words
#include "../util/display.hpp"

namespace Blastn {

f32 dust(string word, u32 pattern_len)
{
    f32 total_score = 0;
    u32 occurrence;
    vector<string> triplets = split_to_words(word, pattern_len);
    dict<string, u32> record;

    for (auto triplet : triplets) {
        // triplet not recorded yet
        if (record.find(triplet) == record.end()) {
            // count occurrences
            occurrence = (u32)std::count(triplets.begin(), triplets.end(), triplet);
            record[triplet] = occurrence * (occurrence - 1) / 2;
        }
    }
    // sum the scores
    for (auto& r : record)
        total_score += r.second;
    
    return total_score / (word.length() - pattern_len);
}

IndexedSequenceMap dust_filter(IndexedSequenceMap& query, f32 threshold, u32 pattern_len)
{
    IndexedSequenceMap result;
    Progress progress{ query.size() };

    // breaks words into subsequences of triplets
    for (auto& qname_seqmap : query) {
        IndexedWordMap temp;
        for (auto& word_indices : qname_seqmap.second) {
            // words that score above the threshold will not be added to the filtered result
            if (dust(word_indices.first, pattern_len) < threshold)
                temp[word_indices.first] = word_indices.second;
        }
        if (!temp.empty())
            result[qname_seqmap.first] = temp;
        
        progress.update();
    }
    return result;
}

} // Blastn