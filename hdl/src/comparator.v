`timescale 1ns / 1ps

/**
 * Compare the current query and subject letters.
 * Assign result to select whether the letters are the same,
 * different, or the query contains a gap.
 */
module Comparator(
        q,          // input current query letter
        s,          // input current subject letter
        result      // output 100 match, 010 mismatch, 001 gap
    );
    
    input  [2:0] q;
    input  [1:0] s;
    output [2:0] result;
    
    assign result = (((q[0] == s[0]) && (q[1] == s[1])) && (q[2] == 0)) ? 3'b100 // match, all bits are the same, gap bit is 0
                  : (
                      ((q[0] != s[0]) || (q[1] != s[1])) && (q[2] == 0) ? 3'b010 // mismatch, not all bits are the same, gap bit is 0
                      : 3'b001 // gap, gap bit is 1
                  );
    
endmodule