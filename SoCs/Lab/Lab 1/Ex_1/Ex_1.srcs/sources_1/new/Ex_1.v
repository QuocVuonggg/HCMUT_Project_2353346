`timescale 1ns / 1ps

module traffic_light_system #(parameter CLOCK_LIMIT = 62500000)(
    input clk,
    input reset_n,
    input [1:0] sw,
    input [1:0] btn,
    output reg [2:0] led_rgb1,
    output reg [2:0] led_rgb2,
    output [3:0] seg0_bcd,
    output [3:0] seg1_bcd,
    output [3:0] seg2_bcd,
    output [3:0] seg3_bcd
);

    wire rst = reset_n;

    localparam S_G1_R2 = 2'b00;
    localparam S_Y1_R2 = 2'b01;
    localparam S_R1_G2 = 2'b10;
    localparam S_R1_Y2 = 2'b11;

    reg [1:0] current_state;
    reg [5:0] timer;

    reg [5:0] set_green_time;
    reg [5:0] set_yellow_time;

    reg [26:0] clk_counter;
    reg clk_1hz;

    reg btn1_prev;
    wire btn1_posedge;

    always @(posedge clk) begin
        btn1_prev <= btn[1];
    end
    assign btn1_posedge = btn[1] && !btn1_prev;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            set_green_time <= 10;
            set_yellow_time <= 3;
        end else begin
            if (sw[0] == 1) begin
                if (btn1_posedge) begin
                    if (sw[1] == 0) begin
                        if (set_green_time < 60) set_green_time <= set_green_time + 1;
                        else set_green_time <= 5;
                    end else begin
                        if (set_yellow_time < 20) set_yellow_time <= set_yellow_time + 1;
                        else set_yellow_time <= 2;
                    end
                end
            end
        end
    end

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            clk_counter <= 0;
            clk_1hz <= 0;
        end else begin
            if (clk_counter >= CLOCK_LIMIT - 1) begin
                clk_counter <= 0;
                clk_1hz <= ~clk_1hz;
            end else begin
                clk_counter <= clk_counter + 1;
            end
        end
    end

    always @(posedge clk_1hz or posedge rst) begin
        if (rst) begin
            current_state <= S_G1_R2;
            timer <= 9;
        end else begin
            if (sw[0] == 0) begin
                if (timer == 0) begin
                    case (current_state)
                        S_G1_R2: begin
                            current_state <= S_Y1_R2;
                            timer <= set_yellow_time - 1;
                        end
                        S_Y1_R2: begin
                            current_state <= S_R1_G2;
                            timer <= set_green_time - 1;
                        end
                        S_R1_G2: begin
                            current_state <= S_R1_Y2;
                            timer <= set_yellow_time - 1;
                        end
                        S_R1_Y2: begin
                            current_state <= S_G1_R2;
                            timer <= set_green_time - 1;
                        end
                    endcase
                end else begin
                    timer <= timer - 1;
                end
            end
        end
    end

    always @(*) begin
        if (sw[0] == 1) begin
             led_rgb1 = 3'b111;
             led_rgb2 = 3'b111;
        end else begin
            case (current_state)
                S_G1_R2: begin led_rgb1 = 3'b010; led_rgb2 = 3'b100; end
                S_Y1_R2: begin led_rgb1 = 3'b001; led_rgb2 = 3'b100; end
                S_R1_G2: begin led_rgb1 = 3'b100; led_rgb2 = 3'b010; end
                S_R1_Y2: begin led_rgb1 = 3'b100; led_rgb2 = 3'b001; end
                default: begin led_rgb1 = 3'b000; led_rgb2 = 3'b000; end
            endcase
        end
    end

    reg [5:0] display_val;

    always @(*) begin
        if (sw[0] == 1) begin
            if (sw[1] == 0) display_val = set_green_time;
            else            display_val = set_yellow_time;
        end else begin
            display_val = timer + 1;
        end
    end

    wire [3:0] tens = display_val / 10;
    wire [3:0] units = display_val % 10;

    assign seg1_bcd = tens;
    assign seg0_bcd = units;
    assign seg2_bcd = 4'd0;
    assign seg3_bcd = 4'd0;

endmodule