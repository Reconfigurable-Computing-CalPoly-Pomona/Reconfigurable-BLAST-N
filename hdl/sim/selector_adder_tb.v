`timescale 1ns / 1ps

module SelectorAdderTb();
        
        reg [1:0] x, score;
        wire [1:0] sum;
        
        SelectorAdder uut(
            .x(x),
            .score(score),
            .sum(sum)
        );
        
        initial begin
            x = 0; score = 2;
            #100;
            x = 1; score = 2;
            #100;
            x = 2; score = 2;
            #100;
            x = 3; score = 2;
            #100;
            
            x = 0; score = -1;
            #100;
            x = 1; score = -1;
            #100;
            x = 2; score = -1;
            #100;
            x = 3; score = -1;
            #100;
                         
        end
    
endmodule 