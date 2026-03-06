`timescale 1ns / 1ps

module tb_led_string_system;

    reg clk;
    reg reset_n;
    reg [3:1] btn;

    wire [3:0] led;
    wire uart_tx_out;

    parameter SIM_CLOCK_FREQ = 200;
    parameter SIM_BAUD_RATE  = 100;

    led_string_system #(
        .CLOCK_FREQ(SIM_CLOCK_FREQ),
        .BAUD_RATE(SIM_BAUD_RATE)
    ) uut (
        .clk(clk),
        .reset_n(reset_n),
        .btn(btn),
        .led(led),
        .uart_tx_out(uart_tx_out)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        $display("Time: %0t | System Reset", $time);
        reset_n = 1;
        btn = 3'b000;
        #100;
        reset_n = 0;

        @(posedge uut.tick_1hz);
        @(posedge uut.tick_1hz);

        $display("Time: %0t | Start Shift Left", $time);
        btn[1] = 1; #100; btn[1] = 0;

        @(posedge uut.tick_1hz);
        @(posedge uut.tick_1hz);
        @(posedge uut.tick_1hz);

        $display("Time: %0t | Start Shift Right", $time);
        btn[2] = 1; #100; btn[2] = 0;

        @(posedge uut.tick_1hz);
        @(posedge uut.tick_1hz);
        @(posedge uut.tick_1hz);

        $display("Time: %0t | Pause", $time);
        btn[3] = 1; #100; btn[3] = 0;

        repeat(4) @(posedge uut.tick_1hz);

        $display("Simulation Finished.");
        $finish;
    end

endmodule