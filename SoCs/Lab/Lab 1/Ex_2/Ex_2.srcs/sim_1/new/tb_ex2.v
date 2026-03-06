`timescale 1ns / 1ps

module tb_led_decoration;

    reg clk;
    reg reset_n;
    reg [1:0] sw;

    wire [3:0] seg0, seg1, seg2, seg3;

    led_decoration_system #( .CLOCK_LIMIT(10) ) uut (
        .clk(clk),
        .reset_n(reset_n),
        .sw(sw),
        .seg0_bcd(seg0), .seg1_bcd(seg1),
        .seg2_bcd(seg2), .seg3_bcd(seg3)
    );

    initial begin
        clk = 0;
        forever #4 clk = ~clk;
    end

    initial begin
        reset_n = 0;
        sw = 2'b00;

        reset_n = 1;
        #20;
        reset_n = 0;

        sw = 2'b00;
        #300;

        reset_n = 1;
        #10;
        reset_n = 0;
        sw = 2'b01;
        #300;

        reset_n = 1;
        #10;
        reset_n = 0;
        sw = 2'b10;
        #400;

        $finish;
    end

endmodule