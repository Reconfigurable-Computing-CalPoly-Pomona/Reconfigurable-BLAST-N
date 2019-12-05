#include <algorithm>
#include <array>
#include <thread>

#include "smith_waterman.hpp"
#include "interface.hpp"

#include "../../util/globals.hpp"

namespace Blastn {

/******************************************************************************
 * 
 * Standard Smith-Waterman
 * 
 */

enum Direction {
    _INVALID,
    LEFT,
    UP,
    DIAG
};

// return the maximum of three values or zero
static inline Greatest greatest_max(s32 left, s32 up, s32 diag)
{
    Greatest max = { 0, 0 };

    if (left > 0) {
        max.value = left;
        max.index = LEFT;
    }
    if (up > max.value) {
        max.value = up;
        max.index = UP;
    }
    if (diag > max.value) {
        max.value = diag;
        max.index = DIAG;
    }

    return max;
}

static inline s32 score_alignment(char alpha, char beta, s32 match, s32 mismatch, s32 gap)
{
    if (alpha == beta)
        return match;
    else if (CHAR_GAP == alpha || CHAR_GAP == beta)
        return gap;
    else
        return mismatch;
}

s32 smith_waterman(string& seq1,
                   string& seq2,
                   s32 match,
                   s32 mismatch,
                   s32 gap,
                   bool just_score)
{

    u32 rows = (u32)seq1.size();
    u32 cols = (u32)seq2.size();
    u32 i = 0;
    u32 j = 0;

    // initialize empty matrices
    Matrix score_matrix, point_matrix;
    score_matrix.reserve(cols);
    point_matrix.reserve(cols);

    // initialize matrices vectors of length rows + 1 with zeros
    for (i = 0; i <= cols; i++) {
        // emblace_back's vector initializer with (length, value to fill)
        score_matrix.emplace_back(rows + 1, 0);
        point_matrix.emplace_back(rows + 1, 0);
    }

    // to fill the matrices
    s32 max_score = 0;
    u32 max_i = 1;
    u32 max_j = 1;
    s32 left, up, diag;
    Greatest greatest;

    // fill score matrix
    for (i = 1; i <= cols; i++) {
        for (j = 1; j <= rows; j++) {

            // determine possible scores of the current cell
            left = score_matrix[i - 1][j] + gap;
            up = score_matrix[i][j - 1] + gap;
            diag = score_matrix[i - 1][j - 1] + score_alignment(seq1[i - 1], seq2[j - 1], match, mismatch, gap);

            // find greatest: load direction into point_matrix, score into score_matrix
            greatest = greatest_max(left, up, diag);
            point_matrix[i][j] = greatest.index;
            score_matrix[i][j] = greatest.value;

            // record high score
            if ((s32)score_matrix[i][j] >= (s32)max_score) {
                max_i = i;
                max_j = j;
                max_score = score_matrix[i][j];
            }
        }
    }

    if (just_score)
        return max_score;

    string temp1, temp2;
    string aligned1 = "";
    string aligned2 = "";
    i = max_i;
    j = max_j;

    while (point_matrix[i][j] != 0) {
        // diagnal movement
        if (point_matrix[i][j] == DIAG) {
            temp1 = seq1[--i];
            temp2 = seq2[--j];
        }
        // upwards movement
        else if (point_matrix[i][j] == UP) {
            temp1 = STR_GAP;
            temp2 = seq2[--j];
        }
        // left movement
        else if (point_matrix[i][j] == LEFT) {
            temp1 = seq1[--i];
            temp2 = STR_GAP;
        }

        // append the chars to the aligned build string
        aligned1.append(temp1);
        aligned2.append(temp2);
    }

    // reverse the strings to forward order
    std::reverse(aligned1.begin(), aligned1.end());
    std::reverse(aligned2.begin(), aligned2.end());

    // accumulator strings for the output
    string output_alignment = "";
    f32 similarity_percent = 0.0;

    for (u32 x = 0; x < rows; x++) {
        string a1{ aligned1[x] };
        string a2{ aligned2[x] };

        // char in both, append to output
        if (a1 == a2) {
            output_alignment.append(a1);
            similarity_percent += 1.0;
        }
        // no match, gap append to output string
        else if (a1 != a2 && a1 != STR_GAP && a2 != STR_GAP) {
            output_alignment.append(STR_GAP);
        }
        // gap in both
        else if (a1 == STR_GAP || a2 == STR_GAP) {
            output_alignment.append(STR_GAP);
        }
    }

    // similarity number to percent
    similarity_percent = (f32)similarity_percent / (f32)aligned1.length() * 100.0f;

    std::cout << "\nScore matrix:" << std::endl;
    std::cout << str(score_matrix) << std::endl;
    std::cout << "\nPoint matrix" << std::endl;
    std::cout << str(point_matrix) << std::endl;

    std::cout << "Sequence A:\n" << seq1 << std::endl;
    std::cout << "Sequence B:\n" << seq2 << std::endl;

    std::cout << "\nSimilarity: " << similarity_percent << std::endl;
    std::cout << "Max Score: " << max_score << std::endl;

    std::cout << "\nAligned A: " << aligned1 << std::endl;
    std::cout << "Aligned B: " << aligned2 << std::endl;
    std::cout << "   Output: " << output_alignment << std::endl;

    return max_score;
}

/******************************************************************************
 * 
 * Reallocate memory on each call, don't preserve the memory
 * 
 */

static inline s32 single_max(s32 left, s32 up, s32 diag)
{
    s32 max = 0;

    if (left > 0)
        max = left;
    if (up > max)
        max = up;
    if (diag > max)
        max = diag;

    return max;
}

s32 smith_waterman_no_preserve(string& seq1, string& seq2, s32 match, s32 mismatch, s32 gap)
{
    u64 rows = seq1.size();
    u64 cols = seq2.size();
    u32 i, j;

    s32 *score_matrix = (s32 *)calloc((cols + 1) * (rows + 1), sizeof(s32));
    s32 max_score = 0;

    // to fill the matrices
    s32 left, up, diag;

    // fill score matrix
    for (i = 1; i <= cols; i++) {
        for (j = 1; j <= rows; j++) {

            // determine possible scores of the current cell
            left = score_matrix[((i - 1) * cols) + j    ] + gap;
            up   = score_matrix[ (i * cols)      + j - 1] + gap;
            diag = score_matrix[((i - 1) * cols) + j - 1] + score_alignment(seq1[i - 1], seq2[j - 1], match, mismatch, gap);

            // find greatest: load direction into point_matrix, score into score_matrix
            score_matrix[i * cols + j] = single_max(left, up, diag);

            // record high score
            if ((s32)score_matrix[i * cols + j] >= (s32)max_score)
                max_score = score_matrix[i * cols + j];
        }
    }
    
    free(score_matrix);
    return max_score;
}

/******************************************************************************
 * 
 * Allocate memory once, preserve memory through different calls
 * 
 */

static s32 *ScoreMatrix = nullptr;
static u64 ScoreMatrixSize = 0;

s32 smith_waterman_preserve(const char *seq1, const char *seq2, s32 match, s32 mismatch, s32 gap, u64 cols, u64 rows)
{
    if ((cols + 1) * (rows + 1) > ScoreMatrixSize) {
        ScoreMatrixSize = (cols + 1) * (rows + 1) * 2;
        if (!ScoreMatrix)
            ScoreMatrix = (s32 *)calloc(ScoreMatrixSize, sizeof(s32));
        else
            ScoreMatrix = (s32 *)realloc(ScoreMatrix, ScoreMatrixSize * sizeof(s32));
    }

    // to fill the matrices
    u32 i, j;
    s32 left, up, diag;
    s32 max_score = 0;

    // fill score matrix
    for (i = 1; i <= cols; i++) {
        for (j = 1; j <= rows; j++) {

            // determine possible scores of the current cell
            left = ScoreMatrix[((i - 1) * cols) + j    ] + gap;
            up   = ScoreMatrix[ (i * cols)      + j - 1] + gap;
            diag = ScoreMatrix[((i - 1) * cols) + j - 1] + score_alignment(seq1[i - 1], seq2[j - 1], match, mismatch, gap);

            // find greatest: load direction into point_matrix, score into score_matrix
            ScoreMatrix[i * cols + j] = single_max(left, up, diag);

            // record high score
            if ((s32)ScoreMatrix[i * cols + j] >= (s32)max_score)
                max_score = ScoreMatrix[i * cols + j];
        }
    }
    
    return max_score;
}

/******************************************************************************
 * 
 * FPGA Interface with Smith-Waterman
 * 
 */

PackedFmt formatted{NULL};
static bool uart_initialized = false;

s32 smith_waterman_fgpa(const char *seq1, const char *seq2, u64 size)
{
    if (!uart_initialized) {
        formatted = PackedFmt(Blastn::UArtPath.c_str());
        uart_initialized = true;
    }
    formatted.pack(seq1, seq2, size);
    formatted.write();
    formatted.read();
    return formatted.result;
}

/******************************************************************************
 * 
 * Multi-Threaded implementation
 * 
 */

// avoid passing
static s32 Match;
static s32 Mismatch;
static s32 Gap;
static u64 Rows;
static u64 Cols;
static string Sequence1;
static string Sequence2;

static void left(s32 *score_matrix)
{
    u64 i, j;

    for (j = 1; j <= Rows; j++) {
        #pragma omp simd
        for (i = 1; i <= Cols; i++) {
            score_matrix[i * Cols + j] = score_matrix[((i - 1) * Cols) + j] + Gap;
        }
    }
}

static void up(s32 *score_matrix)
{
    u64 i, j;

    for (i = 1; i <= Cols; i++) {
        #pragma omp simd
        for (j = 1; j <= Rows; j++) {
            score_matrix[i * Cols + j] = score_matrix[(i * Cols) + j - 1] + Gap;
        }
    }
}

static void diag(s32 *score_matrix)
{
    u64 i, j, pt;

    /**
     * Traverse as follows for a 4x4
     * Note that the matrix is not guaranteed to be square, just ignore the 2 corners
     * XX XX XX XX
     * XX 11 12 13
     * XX 21 22 23
     * XX 31 32 33
     * 
     * Where the pairs are repeated diagonal down left \
     * With starting points: (31), (21, 32), (11, 22, 33), (12, 23), (13)
     */

    // hold the starting locations x1, y1, x2, y2, ...
    u64 num_pts = 2 * ((Cols - 1) + (Rows - 1) - 1);
    s32 *start_pts = (s32 *)calloc(num_pts, sizeof(s32));
    if (!start_pts) {
        std::cerr << "Error: Failed to allocate memory for diagonal Smith-Waterman matrix." << std::endl;
        std::exit(-1);
    }

    // claim all starting locations
    for (i = Cols - 1, j = 1, pt = 0; pt < num_pts; pt += 2) {
        start_pts[pt] = i;
        start_pts[pt + 1] = j;

        if (i != 0)
            i--;
        else
            j++;
    }

    // traverse for each starting point
    #pragma omp simd
    for (pt = 0; pt < num_pts; pt++) {
        // traverse diagonally
        for (i = start_pts[pt], j = start_pts[pt + 1]; i < Cols && j < Rows; i++, j++) {
            score_matrix[i * Cols + j] = score_matrix[((i - 1) * Cols) + j - 1]
                + score_alignment(Sequence1[i - 1], Sequence2[j - 1], Match, Mismatch, Gap);
        }
    }

    free(start_pts);
}

s32 smith_waterman_mt(string& seq1, string& seq2, s32 match, s32 mismatch, s32 gap)
{
    Match = match;
    Mismatch = mismatch;
    Gap = gap;
    Rows = seq1.size();
    Cols = seq2.size();
    Sequence1 = seq1.c_str();
    Sequence2 = seq2.c_str();

    s32 max_score = 0;

    // create shared memory, store scores in each 1/3
    u64 shm_size = (Cols + 1) * (Rows + 1);
    s32 *shm = (s32 *)calloc(3 * shm_size, sizeof(u32));
    if (!shm) {
        std::cerr << "Error: Failed allocate memory for Smith-Waterman matrix." << std::endl;
        std::exit(-1);
    }
    s32 *score_left = shm;
    s32 *score_up   = shm + shm_size;
    s32 *score_diag = shm + 2 * shm_size;

    // threads...
    std::thread calc_left(left, score_left);
    std::thread   calc_up(up,   score_up);

    // no need to multi thread all, diag should be slowest, run in this thread
    diag(score_diag);
    calc_left.join();
    calc_up.join();

    // find greatest value, traverse backwards,
    // traverse greater percent of the array
    for (u64 i = shm_size - 1; i >= shm_size / 4; i--) {
        if (score_left[i] >= max_score)
            max_score = score_left[i];
        if (score_up[i] >= max_score)
            max_score = score_up[i];
        if (score_diag[i] >= max_score)
            max_score = score_diag[i];
    }

    free(shm);
    return max_score;
}

} // Blastn