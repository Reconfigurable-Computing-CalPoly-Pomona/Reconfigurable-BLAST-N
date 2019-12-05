module AlignScoreTb();
    reg  [2:0] q;
    reg  [1:0] s, match, mismatch, gap, diag;
    wire [1:0] score;
    
    AlignScore uut(
        .q(q),
        .s(s),
        .match(match),
        .mismatch(mismatch),
        .gap(gap),
        .diag(diag),
        .score(score)
   );
   
   initial begin
   // if q is even diag score is 2 otherwise it is 1 for testing purposes
   
   q = 0; s = 0; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 0; s = 1; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 0; s = 2; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 0; s = 3; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 1; s = 0; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 1; s = 1; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 1; s = 2; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 1; s = 3; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 2; s = 0; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 2; s = 1; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 2; s = 2; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 2; s = 3; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100; 
   q = 3; s = 0; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 3; s = 1; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 3; s = 2; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 3; s = 3; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 4; s = 0; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 4; s = 1; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100; 
   q = 4; s = 2; match = 2; mismatch = -1; gap = 0; diag = 2; 
   #100;
   q = 4; s = 3; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 5; s = 0; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 5; s = 1; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 5; s = 2; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 5; s = 3; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 6; s = 0; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 6; s = 1; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 6; s = 2; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 6; s = 3; match = 2; mismatch = -1; gap = 0; diag = 2;
   #100;
   q = 7; s = 0; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 7; s = 1; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 7; s = 2; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100;
   q = 7; s = 3; match = 2; mismatch = -1; gap = 0; diag = 1;
   #100; 
      
   end
endmodule