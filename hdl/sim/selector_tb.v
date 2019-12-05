`timescale 1ns / 1ps

module SelectorTb();
    reg  [2:0] sel;
    reg  [1:0] match, mismatch, gap;
    wire [1:0] result;
    
    Selector uut(
        .sel(sel),
        .match(match),
        .mismatch(mismatch),
        .gap(gap),
        .result(result)
    );
    
    initial begin
        // normally match = 2, mismatch and gap = -1; gap is 0 for testing purposes
        sel = 3'b100; match = 2; mismatch = -1; gap = 0; 
        #100;
        sel = 3'b010; match = 2; mismatch = -1; gap = 0; 
        #100;
        sel = 3'b001; match = 2; mismatch = -1; gap = 0; 
        #100;
    end
endmodule