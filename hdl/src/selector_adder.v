`timescale 1ns / 1ps

/**
 * Module to come after Selector. Add the selected score with the diagonal up left value
 * to calculate the Smith-Waterman diagonal score. The result of these modules will
 * score based on whether the current query and subject letter were a match, mismatch, or gap.
 */
module SelectorAdder(
        x,      // input diagonal value
        score,  // input selected score depending on match, mismatch, or gap
        sum     // output sum
    );
    
    input [1:0] x;
    input [1:0] score;
    output [1:0] sum;
    
    assign sum = ((score == 2'b10) && (x == 2'b00)) ? 2'b10 : ((score == 2'b11) && (x[1] == 0)) ? 2'b00 
                : ((score == 2'b11) && (x[1] == 1)) ? x + score : 2'b11;
    
endmodule