#pragma once

#include "../util/types.hpp"

namespace Blastn {

/**
 * @brief Sort the extended pairs by query id, then by E value
 */
vector<HSP> sort(vector<HSP>& hsps);

} // Blastn