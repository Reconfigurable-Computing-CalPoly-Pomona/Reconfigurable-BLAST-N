#include "blastn.hpp"
#include "../util/globals.hpp"
#include "../util/test.hpp"
#include "../util/display.hpp"

namespace Blastn {

static std::string argparse(vector<string> arguments, std::string arg)
{
    for (size_t i = 0; i < arguments.size(); i++) {
        if (arg == arguments[i]) {
            if (i + 1 >= arguments.size()) {
                std::cerr << "Failure: INVALID argument usage: " << arg << std::endl;
                std::exit(-1);
            }
            else
                return arguments[i + 1];
        }
    }
    return Blastn::INVALID;
}

int test(std::vector<std::string> args)
{
    string a;
    a = argparse(args, "-test");
    if (a == Blastn::INVALID) {
        std::cerr << "Error: Unknown test function argument." << std::endl;
        std::exit(-1);
    }
    
    if (a == "dust")
        Test::dust();
    else if (a == "extend")
        Test::extend();
    else if (a == "match")
        Test::match();
    else if (a == "pairs")
        Test::pairs();
    else if (a == "sequence")
        Test::sequence();
    else if (a == "sw" || a == "smith waterman")
        Test::smith_waterman();
    else if (a == "sort")
        Test::sort();
    else if (a == "split")
        Test::split();
    else
        std::cout << "Unknown test function: " << a << std::endl;

    return 0;
}

static void align(std::string query_file, std::string subject_file)
{
    Timer timer{ "Blastn" };
    timer.start();

    /**
     * Data formatting
     */

    std::cout << "[1 / 11] Reading " << query_file << "..." << std::endl;
    auto query = Blastn::build_sequence(query_file, Blastn::Seperator);
    std::cout << "[2 / 11] Formatting " << query.size() << " query entries..." << std::endl;
    auto query_prepared = Blastn::split_sequence(query, Blastn::SplitLength);
    std::cout << std::endl;

    std::cout << "[3 / 11] Reading " << subject_file << "..." << std::endl;
    auto subject = Blastn::build_sequence(subject_file, Blastn::Seperator);
    std::cout << "[4 / 11] Formatting " << subject.size() << " subject entries..." << std::endl;
    auto subject_prepared = Blastn::split_sequence(subject, Blastn::SplitLength);
    std::cout << std::endl;

    // needed for E-value calculation
    u64 subject_length = sequence_length(subject);

    /**
     * Dust filtering
     */

    std::cout << "[5 / 11] Dust Complexity Filtering " << query.size() << " query entries..." << std::endl;
    auto query_dustfiltered = Blastn::dust_filter(query_prepared, Blastn::DustThreshold, Blastn::DustPatternLength);
    std::cout << std::endl;

    /**
     * Exact matches, adjacent pairs, extending pairs, and sorting extended pairs
     * all on smaller subject subsets
     */

    std::cout << "[6 / 11] Matching " << query.size() << " query entries against " << subject.size() << " subject entries..." << std::endl;
    auto exact_matches = Blastn::match_filter(query_dustfiltered, subject_prepared);
    std::cout << std::endl;

    std::cout << "[7 / 11] Pairing " << exact_matches.size() << " words with each other..." << std::endl;
    auto adjacent_pairs = Blastn::pair_filter(exact_matches, query);
    std::cout << std::endl;

    std::cout << "[8 / 11] Extending " << adjacent_pairs.size() << " paired words..." << std::endl;

    int sw_flag = SW::PRESERVE_MEM;
    if (Blastn::UArtFPGA)
        sw_flag = SW::FPGA;
    
    auto extended_pairs = Blastn::extend_filter(adjacent_pairs, query, subject, Blastn::SwMatch, Blastn::SwMismatch, Blastn::SwGap, Blastn::SwRatio, sw_flag);
    std::cout << std::endl;

    /**
     * Get HSP's from the extended pairs
     */

    std::cout << "[9 / 11] Formatting " << extended_pairs.size() << " HSPs..." << std::endl;
    auto hsps = Blastn::format_hsps(extended_pairs, query, subject, Blastn::Lambda, Blastn::Kappa, subject_length);
    std::cout << std::endl;

    std::cout << "[10 / 11] Sorting " << hsps.size() << " HSPs..." << std::endl;
    auto sorted_hsps = Blastn::sort(hsps);
    std::cout << std::endl;

    /**
     * Blastn finished, write to file
     */

    std::cout << "[11 / 11] Writing to file " << Blastn::OutputFile << std::endl;
    string formatted_output = Blastn::format_output(sorted_hsps, query, subject, Blastn::OutputSpacing);
    Blastn::write_output(formatted_output, Blastn::OutputFile);

    std::cout << std::endl;

    /**
     * Timer stats
     */
    timer.stop();
}

int blastn(std::vector<std::string> args)
{
    // arg output
    string a;

    // files
    if ((a = argparse(args, "-query")) != Blastn::INVALID)
        Blastn::QueryFile = a;
    if ((a = argparse(args, "-subject")) != Blastn::INVALID)
        Blastn::SubjectFile = a;
    if ((a = argparse(args, "-out")) != Blastn::INVALID)
        Blastn::OutputFile = a;

    // query/subject file descriptions
    if ((a = argparse(args, "-sep")) != Blastn::INVALID)
        Blastn::Seperator = (char)a[0];
    if ((a = argparse(args, "-word-length")) != Blastn::INVALID)
        Blastn::SplitLength = atoi(a.c_str());
    
    // smith waterman
    if ((a = argparse(args, "-sw-minscore")) != Blastn::INVALID)
        Blastn::SwMinscore = atoi(a.c_str());
    if ((a = argparse(args, "-sw-match")) != Blastn::INVALID)
        Blastn::SwMatch = atoi(a.c_str());
    if ((a = argparse(args, "-sw-mismatch")) != Blastn::INVALID)
        Blastn::SwMismatch = atoi(a.c_str());
    if ((a = argparse(args, "-sw-gap")) != Blastn::INVALID)
        Blastn::SwGap = atoi(a.c_str());
    
    Blastn::SwRatio = (f32)SwMinscore / (f32)(SplitLength * SwMatch);

    // dust
    if ((a = argparse(args, "-dust-thresh")) != Blastn::INVALID)
        Blastn::DustThreshold = (f32)atof(a.c_str());
    if ((a = argparse(args, "-dust-length")) != Blastn::INVALID)
        Blastn::DustPatternLength = atoi(a.c_str());

    if ((a = argparse(args, "-lambda")) != Blastn::INVALID)
        Blastn::Lambda = (f32)atof(a.c_str());
    if ((a = argparse(args, "-kappa")) != Blastn::INVALID)
        Blastn::Kappa = (f32)atof(a.c_str());

    // output spacing
    if ((a = argparse(args, "-spacing")) != Blastn::INVALID)
        Blastn::OutputSpacing = atoi(a.c_str());

    if ((a = argparse(args, "-fpga")) != Blastn::INVALID) {
        Blastn::UArtFPGA = true;
        Blastn::UArtPath = a;
        #ifndef _WIN32
            std::cerr << "Error: FPGA connection to UART was only designed for the Windows 10 operating system." << std::endl;
            std::exit(-1);
        #endif
    }
    

    align(Blastn::QueryFile, Blastn::SubjectFile);
    return 0;
}

} // Blastn