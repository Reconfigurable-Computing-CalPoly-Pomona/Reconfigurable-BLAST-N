`timescale 1ns / 1ps

/**
 * Determine the diagonal score based on all submodules chained together.
 */
module AlignScore(
        q,          // input current query letter
        s,          // input current subject letter
        diag,       // input diagonal up left score
        match,      // input global match score
        mismatch,   // input global mismatch score
        gap,        // input global gap score
        score       // output score of the current cell
    );
    
    input [2:0] q;
    input [1:0] s;
    input [1:0] match;
    input [1:0] mismatch;
    input [1:0] gap;
    input [1:0] diag;
    output [1:0] score;
    
    wire [2:0] cmp_result;
    wire [1:0] sel_result;
    
    Comparator mod_cmp(
        .q(q),              // current query letter
        .s(s),              // current subject letter
        .result(cmp_result) // output 100 match, 010 mismatch, 001 gap
    );
    
    // Select score depending if the current query and
    // subject letter was a match, mismtach, or gap.
    Selector mod_sel(
        .sel(cmp_result),
        .match(match),
        .mismatch(mismatch),
        .gap(gap),
        .result(sel_result)
    );
    
    SelectorAdder mod_seladd(
        .x(diag),
        .score(sel_result),
        .sum(score)
    );
    
endmodule 