#pragma once

#include "../util/types.hpp"

namespace Blastn {

/**
 * @brief Prepare the extended pairs for being sorted
 * @param extended_pairs The pairs which were previously extended and need to be scored
 * @param query          The original queries mapped to their sequences. Length needed for
 *                       E-value calculation
 * @param subject        The original subjects mapped to their sequences. Length needed for
 *                       E-value calculation
 * @param lambda         Bitscore input parameter
 * @param kappa          Bitscore input parameter
 * @param subject_length The cumulative length of all subjects together
 * @return High scoring pairs with score info, ready to be written to file
 */
vector<HSP> format_hsps(ExtendedSequenceMap& extended_pairs,
                        SequenceMap& query,
                        SequenceMap& subject,
                        f32 lambda,
                        f32 kappa,
                        u64 subject_length);

} // Blastn