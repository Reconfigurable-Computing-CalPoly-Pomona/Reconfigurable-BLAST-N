#include "globals.hpp"

namespace Blastn {

bool UArtFPGA = false;
string UArtPath = "COM4";

// .fa and .fasta line seperator
char Seperator = '>';
// word length
u32 SplitLength = 11;

// min smith waterman score before filtering out
s32 SwMinscore = 20;
// smith waterman match score
s32 SwMatch = 2;
// smith waterman mismatch score
s32 SwMismatch = -1;
// smith waterman gap score
s32 SwGap = -1;
// ratio of max score to length
f32 SwRatio = 0.0f; // SwMinscore / (SplitLength * SwMatch)

// dust threshold score before filtering out
f32 DustThreshold = 0.1f;
// length of patterns for dust to look for
u32 DustPatternLength = 3;

// bit score calculation
f32 Lambda = 0.6225f;
f32 Kappa = 0.041f;

//string QueryFile   = "../data/SRR7236689--ARG830.fa";
string SubjectFile = "../data/Gn-SRR7236689_contigs.fasta";
string OutputFile  = "result.txt";

string /*Test*/ QueryFile = "../data/query_small.fa";
//string /*Test*/ SubjectFile  = "../data/subject_small.fasta";

u32 OutputSpacing = 6;

} // Blastn