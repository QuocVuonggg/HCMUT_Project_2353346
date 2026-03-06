`timescale 1ns / 1ps

module tb_system_clean;

    reg [6:0] btn;
    wire [31:0] sum;
    wire [7:0] led;

    cla uut (
        .a(32'd26),
        .b({25'd0, btn}),
        .cin(1'b0),
        .sum(sum)
    );

    assign led = sum[7:0];

    initial begin
        btn = 0;
        #10;

        btn = 1;
        #10;

        btn = 4;
        #10;

        btn = 10;
        #10;

        btn = 74;
        #10;

        $finish;
    end

endmodule