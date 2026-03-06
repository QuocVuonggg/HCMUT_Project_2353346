`timescale 1ns / 1ps

module tb_divider;

    reg [31:0] i_dividend;
    reg [31:0] i_divisor;
    wire [31:0] o_quotient;
    wire [31:0] o_remainder;

    divider_unsigned uut (
        .i_dividend(i_dividend),
        .i_divisor(i_divisor),
        .o_quotient(o_quotient),
        .o_remainder(o_remainder)
    );

    initial begin
        i_dividend = 4; i_divisor = 2; #10;

        i_dividend = 10; i_divisor = 4; #10;

        i_dividend = 100; i_divisor = 3; #10;
        
        i_dividend = 32'd2000000; i_divisor = 32'd5; #10;

        i_dividend = 12345678; i_divisor = 1; #10;

        i_dividend = 50; i_divisor = 100; #10;

        i_dividend = 0; i_divisor = 999; #10;

        i_dividend = 32'd987654321; i_divisor = 32'd987654321; #10;

        i_dividend = 32'hFFFFFFFF; i_divisor = 2; #10;

        $finish; 
    end

endmodule