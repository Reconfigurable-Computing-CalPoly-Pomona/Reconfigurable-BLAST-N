#pragma once

#include <string>

namespace Blastn {

/**
 * Byte packing utilities
 */

// MUST be divisible by 4
// The equivalent is g_SIZE in VHDL
#define SW_MAX_BYTES 12
constexpr size_t BaudRate = 1000000;

enum Pack {
    A = 0b00,
    C = 0b01,
    G = 0b10,
    T = 0b11
};

auto pack_find = [&](char letter) {
    switch (letter) {
    case 'A': case 'a':
        return Pack::A;
    case 'C': case 'c':
        return Pack::C;
    case 'G': case 'g':
        return Pack::G;
    case 'T': case 't':
        return Pack::T;
    default:
        fprintf(stderr, "Error: Invalid Smith-Waterman letter: %x\n", letter);
        std::exit(-1);
    }
};

/**
 * Specify which smith waterman implementation to use in extend
 */
enum SW {
    NO_PRESERVE_MEM,
    PRESERVE_MEM,
    MULTI_THREAD,
    FPGA,
};

/**
 * Constants used throughout Blastn
 */

const std::string STR_GAP = "-";
const char CHAR_GAP = '-';
const std::string INVALID = "\0";

}
