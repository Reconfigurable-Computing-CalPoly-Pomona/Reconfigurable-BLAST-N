#include <fstream>
#include <sstream>
#include <iomanip>

#include "output.hpp"
#include "../util/display.hpp"

namespace Blastn {

static const string SEPERATOR = "\t";

string format_output(vector<HSP>& hsps, SequenceMap& query, SequenceMap& subject, u32 precision)
{
    std::ostringstream stream;
    u64 qsize = 0;
    u64 ssize = 0;

    // find longest query name
    for (auto& qname_seq : query) {
        if (qname_seq.first.size() > qsize)
            qsize = qname_seq.first.size();
    }
    // longest subject name
    for (auto& sname_seq : subject) {
        if (sname_seq.first.size() > ssize)
            ssize = sname_seq.first.size();
    }

    for (auto& hsp : hsps) {
        stream << std::setw(qsize)     << hsp.query_id      << SEPERATOR;
        stream << std::setw(ssize)     << hsp.subject_id    << SEPERATOR;
        stream << std::fixed  << hsp.percentage_id * 100.0f << SEPERATOR;
        stream << std::setw(precision) << hsp.matches       << SEPERATOR;
        stream << std::setw(precision) << hsp.mismatches    << SEPERATOR;
        stream << std::setw(precision) << hsp.gaps          << SEPERATOR;
        stream << std::setw(precision) << hsp.query_start   << SEPERATOR;
        stream << std::setw(precision) << hsp.query_end     << SEPERATOR;
        stream << std::setw(precision) << hsp.subject_start << SEPERATOR;
        stream << std::setw(precision) << hsp.subject_end   << SEPERATOR;
        stream << std::scientific      << hsp.evalue        << SEPERATOR;
        stream << std::fixed           << hsp.bitscore      << std::endl;

    }
    return stream.str();
}

void write_output(string& data, string& filename)
{
    std::ofstream output_file{ filename };

    if (output_file.is_open()) {
        output_file.write(data.c_str(), data.size());
        output_file.close();
    }
    else {
        std::cerr << "Error: Failed to open output file: " << filename << std::endl;
        std::exit(-1);
    }
}

} // Blastn