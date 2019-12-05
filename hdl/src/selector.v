`timescale 1ns / 1ps

/**
 * Module to come after Comparator. Depending on what the comparator picked,
 * feed the corresponding score value through.
 */
module Selector(
        sel,        // comparator results, 3 bit input
        match,      // input feed through if 100
        mismatch,   // input feed through if 010
        gap,        // input feed through if 001
        result      // the selected score
    );
    
    input  [2:0] sel;
    input  [1:0] match;
    input  [1:0] mismatch;
    input  [1:0] gap;
    output [1:0] result;
    
    assign result = (sel[2] == 1) ? match : ((sel[1] == 1) ? mismatch : gap);
    
endmodule