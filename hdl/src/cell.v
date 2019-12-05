`timescale 1ns / 1ps

/**
 * An individual cell in the Smith-Waterman score matrix. This
 * scores the up, left, and diagonal up left choices, and selects
 * the greatest.
 */
module Cell(
        s,          // input current subject letter
        q,          // input current query letter
        match,      // input match score
        mismatch,   // input mismatch score
        gap,        // input gap score
        diag,       // input diagonal value
        up,         // input up value
        left,       // input left value
        score       // output max of diagonal, up, or left
    );
    
    input  [1:0] s;
    input  [2:0] q;
    input  [1:0] match;
    input  [1:0] mismatch;
    input  [1:0] gap;
    input  [1:0] diag;
    input  [1:0] up;
    input  [1:0] left;
    output [1:0] score;
    
    wire [1:0] sum_left;
    wire [1:0] sum_up;
    wire [1:0] sum_diag;
    
    GapAdder mod_left(
        .x(left),       // input from left or up
        .sum(sum_left)  // output sum of x and gap
    );
    
    GapAdder mod_up(
        .x(up),         // input from left or up
        .sum(sum_up)    // output sum of x and gap
    );
    
    // diagonal score
    AlignScore mod_as(
        .q(q),
        .s(s),
        .diag(diag),
        .match(match),
        .mismatch(mismatch),
        .gap(gap),
        .score(sum_diag)
    );
    
    // max value between all score directions
    assign score = (sum_left > sum_up) ? ((sum_left > sum_diag) ? sum_left : sum_diag) : ((sum_up > sum_diag) ? sum_up : sum_diag);
    
endmodule