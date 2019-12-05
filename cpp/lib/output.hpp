#pragma once

#include "../util/types.hpp"

namespace Blastn {

/**
 * @brief Convert the high scoring pairs into a string as the standard Blastn output:
 *        Query ID    Subject ID    Percentage ID    Matches    Mismatches    Gaps    Query Start    Query End    Subject Start    Subject End    E Value    Bitscore
 * @param hsps      The high scoring pairs to format into a string
 * @param query     Input query to find the longest sequence name for formatting
 * @param subject   Input subject to find the longest sequence name for formatting
 * @param precision The amount of space to have between each column
 * @return The high scoring pairs in standard Blastn output as a string
 */
string format_output(vector<HSP>& hsps, SequenceMap& query, SequenceMap& subject, u32 precision);

/**
 * @brief Write the result of format_output to a specified file
 */
void write_output(string& data, string& output_file);

} // Blastn