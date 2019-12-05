#include "match.hpp"
#include "prepare.hpp"
#include "../util/display.hpp"

namespace Blastn {

MatchedSequenceMap match_filter(IndexedSequenceMap& query, IndexedSequenceMap& subject)
{
    MatchedSequenceMap exact_matches;
    Progress progress { subject.size() };

    // traverse the subject's IndexedSequenceMap
    for (auto& sname_wordmap : subject) {
        MatchedMatchesMap matches;
        // traverse the query IndexedSequenceMap, create Match objects and insert to exact_matches
        for (auto& qname_wordmap : query) {
            for (auto& qword_indices : qname_wordmap.second) {
                // skip if the subject doesn't have query word
                if (sname_wordmap.second.find(qword_indices.first) == sname_wordmap.second.end())
                    continue;
                
                // the current word in the query is also in subject, vector<Match> already exists
                matches[qname_wordmap.first].emplace_back(
                    qword_indices.first,
                    sname_wordmap.second[qword_indices.first],
                    qword_indices.second
                );
            }
        }

        // record if there were matches found
        if (!matches.empty())
            exact_matches[sname_wordmap.first] = matches;

        progress.update();
    }

    return exact_matches;
}

} // Blastn