module CellTb();
    
    reg  [2:0] q;
    reg  [1:0] s, match, mismatch, gap, diag, up, left;
    wire [1:0] score;
    
    Cell uut(
        .s(s),
        .q(q),
        .match(match),
        .mismatch(mismatch),
        .gap(gap),
        .diag(diag),
        .up(up),
        .left(left),
        .score(score)
    );
    
    initial begin
    
    s = 0; q = 0; match = 2; mismatch = -1; gap = -1; diag = 0; up = 0; left = 0;
    #100;
    
    s = 0; q = 0; match = 2; mismatch = -1; gap = -1; diag = 2; up = 1; left = 1;
    #100;
    
    s = 0; q = 1; match = 2; mismatch = -1; gap = -1; diag = 2; up = 3; left = 1;
    #100;

    s = 1; q = 4; match = 2; mismatch = -1; gap = -1; diag = 0; up = 1; left = 2;
    #100;
    
    s = 2; q = 5; match = 2; mismatch = -1; gap = -1; diag = 3; up = 2; left = 2;
    #100;
 
    end

endmodule