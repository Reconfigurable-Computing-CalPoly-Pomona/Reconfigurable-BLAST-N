`timescale 1ns / 1ps

/**
 * Add the up or left cell's value with the gap score.
 */
module GapAdder(
        x,      // input from left or up
        sum     // output sum of x and gap
    );
    
    input  [1:0] x;
    output [1:0] sum;
    
    /**
     * Gap is always -1 == 2'b11
     * 
     * Truth table:
     * 
     * x1 x0 g1 g0 | s1 s0
     * 0  0  1  1  | 0  0
     * 0  1  1  1  | 0  0
     * 1  0  1  1  | 0  1
     * 1  1  1  1  | 1  0
     * 
     * Result is zero if x[1] == 0
     */
    
    assign sum = x[1] == 0 ? 2'b00 : (x[0] == 0 ? 2'b01 : 2'b10);
    
endmodule