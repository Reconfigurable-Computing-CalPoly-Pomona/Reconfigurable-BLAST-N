 `timescale 1ns / 1ps
 
 module GapAdderTb();
    reg  [1:0] x;
    wire [1:0] sum;
    
    GapAdder uut(
        .x(x),
        .sum(sum)
    );
    
    initial begin
        x = 0;
        #100;
        x = 1;
        #100;
        x = 2;
        #100;
        x = 3;
        #100;
        $finish;
    end
endmodule 