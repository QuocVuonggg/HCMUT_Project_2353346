`timescale 1ns / 1ps

module tb_traffic_light;

    reg clk;
    reg reset_n;
    reg [1:0] sw;
    reg [1:0] btn;

    wire [2:0] led_rgb1;
    wire [2:0] led_rgb2;
    wire [3:0] seg0, seg1, seg2, seg3;

    traffic_light_system #( .CLOCK_LIMIT(10) ) uut (
        .clk(clk),
        .reset_n(reset_n),
        .sw(sw),
        .btn(btn),
        .led_rgb1(led_rgb1), .led_rgb2(led_rgb2),
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
        btn = 2'b00;

        reset_n = 1;
        #100;
        reset_n = 0;

        #1000;

        sw = 2'b01;
        #100;

        btn = 2'b10;
        #20;
        btn = 2'b00;
        #20;

        btn = 2'b10;
        #20;
        btn = 2'b00;
        #20;

        sw = 2'b11;
        #50;

        btn = 2'b10;
        #20;
        btn = 2'b00;
        #20;

        sw = 2'b00;

        #5000;

        $finish;
    end

endmodule