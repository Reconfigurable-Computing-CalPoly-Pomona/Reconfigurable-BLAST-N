#include <algorithm> // sort

#include "sort.hpp"

namespace Blastn {

vector<HSP> sort(vector<HSP>& hsps)
{
    // sort by score
    std::sort(hsps.begin(), hsps.end(), [&](const HSP lhs, const HSP rhs)
    {
        if (lhs.query_id == rhs.query_id)
            return lhs.evalue < rhs.evalue;
        return lhs.query_id < rhs.query_id;
    });
    
    return hsps;
}

} // Blastn