#include "extend.hpp"
#include "smith_waterman.hpp"
#include "../../util/display.hpp"

namespace Blastn {

#define MIN(v1, v2) (((v1) > (v2)) ? (v2) : (v1))
#define MAX(v1, v2) (((v1) < (v2)) ? (v2) : (v1))

/* 
 *        ex) Query:     G'TC'TGAA'CT'GAGC
 *            Subject:  AG'TC'TGATGA'CT'GGGGAACTCGA
 *            Left Word:  'TC'
 *            Right Word: 'CT'
 * 
 *            Steps:
 *          1. Query:    G'TC'                 -> score high enough? (extend left)
 *             Subject:  G'TC'
 *          2. Query:    G'TC'T                -> score high enough? (extend right)
 *             Subject:  G'TC'T
 *          3. Query:    G'TC'TGAA             -> score high enough? (extend right more...)
 *             Subject:  G'TC'TGAT
 *          4. Query:    G'TC'TGAA--           -> score high enough? (add gaps)
 *             Subject:  G'TC'TGATGA
 *          5. Query:    G'TC'TGAA--'CT'       -> score high enough? (add right word)
 *             Subject:  G'TC'TGATGA'CT'
 *          6. Query:    G'TC'TGAA--'CT'GAGC   -> score high enough? (extend right more...)
 *             Subject:  G'TC'TGATGA'CT'GGGG
 *          7. Done!
 */

static inline s32 sw_score(string qextended, string sextended, s32 match, s32 mismatch, s32 gap, u32 flag)
{
    switch (flag) {
        case SW::FPGA:
            if (qextended.size() <= SW_MAX_BYTES * 4)
                return smith_waterman_fgpa(qextended.c_str(), sextended.c_str(), qextended.size());
        case SW::PRESERVE_MEM:
            return smith_waterman_preserve(qextended.c_str(), sextended.c_str(), match, mismatch, gap, sextended.size(), qextended.size());
        case SW::NO_PRESERVE_MEM:
            return smith_waterman_no_preserve(qextended, sextended, match, mismatch, gap);
        case SW::MULTI_THREAD:
            return smith_waterman_mt(qextended, sextended, match, mismatch, gap);
        default:
            std::cerr << "Error: INVALID flag for Extending " << flag << std::endl;
            std::exit(-1);
    }
}

Extended extend_and_score(AdjacentPair pair,
                          string& query,
                          string& subject,
                          s32 match,
                          s32 mismatch,
                          s32 gap,
                          f32 ratio,
                          bool score,
                          bool printing,
                          u32 flag)
{
    // find left most indices
    u32 sleftindex  = MIN(pair.sindex1, pair.sindex2);
    u32 qleftindex  = MIN(pair.qindex1, pair.qindex2);
    u32 srightindex = MAX(pair.sindex1, pair.sindex2);
    u32 qrightindex = MAX(pair.qindex1, pair.qindex2);

    // build string
    string qextended = query.substr(qleftindex, pair.length);
    string sextended = subject.substr(sleftindex, pair.length);

    // running Smith Waterman score
    s32 running_score = 0;

    // extend left
    s32 qexindex = qleftindex;
    s32 sexindex = sleftindex;
    while (qexindex - 1 >= 0 && sexindex - 1 >= 0) {
        qexindex--; sexindex--;
        qextended = query[qexindex] + qextended;
        sextended = subject[sexindex] + sextended;

        if (score) {
            running_score = sw_score(qextended, sextended, match, mismatch, gap, flag);

            if (running_score < ratio * qextended.size() * match)
                return Extended{ INVALID, 0, 0, 0 };
        }
    }

    // the left-most index in the subject
    u32 sindex = (u32)sexindex;
    u32 qindex = (u32)qexindex;
    
    // extend left pair to the right
    qexindex = qleftindex + pair.length - 1;
    sexindex = sleftindex + pair.length - 1;
    while ((u32)qexindex + 1 < qrightindex && (u32)sexindex + 1 < srightindex) {
        qexindex++; sexindex++;
        qextended = qextended + query[qexindex];
        sextended = sextended + subject[sexindex];
    }

    // extend right with gaps until qextended aligns with subject
    while ((u32)sexindex + 1 < srightindex) {
        qexindex++; sexindex++;
        qextended = qextended + STR_GAP;
        sextended = sextended + subject[sexindex];
    }

    // append the right pair
    qextended = qextended + query.substr(qrightindex, pair.length);
    sextended = sextended + subject.substr(srightindex, pair.length);

    // extend right
    qexindex = qrightindex + pair.length - 1;
    sexindex = srightindex + pair.length - 1;
    while ((s32)(qexindex + 1) < (s32)query.size() && (s32)(sexindex + 1) < (s32)subject.size()) {
        qexindex++; sexindex++;
        qextended = qextended + query[qexindex];
        sextended = sextended + subject[sexindex];

        if (score) {
            running_score = sw_score(qextended, sextended, match, mismatch, gap, flag);

            if (running_score < ratio * qextended.size() * match)
                return Extended{ INVALID, 0, 0, 0 };
        }
    }

    if (printing) {
        std::cout << "Subject Ext:\t" << sextended << std::endl;
        std::cout << "Quer Ext:\t" << qextended << std::endl;
    }
    return Extended{ qextended, sindex, qindex, running_score };
}

ExtendedSequenceMap extend_filter(PairedSequenceMap& pairs,
                                  SequenceMap& query,
                                  SequenceMap& subject,
                                  s32 match,
                                  s32 mismatch,
                                  s32 gap,
                                  f32 ratio,
                                  int flag)
{
    ExtendedSequenceMap result;
    bool found = false;
    Progress progress{ pairs.size() };

    for (auto& sname_quermap : pairs) {
        ExtendedPairsMap temp;
        for (auto& qname_pairvec : sname_quermap.second) {
            for (auto adjacent_pair : qname_pairvec.second) {

                Extended ext = extend_and_score(adjacent_pair,
                                                query[qname_pairvec.first],
                                                subject[sname_quermap.first], // sw?  print? flag?
                                                match, mismatch, gap, ratio,   true, false, flag);
                // the word scored below the minscore
                if (ext.extended_pair == INVALID)
                    continue;
                
                // check to see if the extended pair was inserted yet
                found = false;
                for (auto& e : temp[qname_pairvec.first]) {
                    if (ext.sindex == e.sindex) {
                        found = true;
                        break;
                    }
                }
                // don't record the pair if it was already found
                if (found)
                    continue;

                // vector<Extended> already initialized
                temp[qname_pairvec.first].push_back(ext);
            }
        }
        if (!temp.empty())
            result[sname_quermap.first] = temp;

        progress.update();
    }
    return result;
}

} // Blastn