`timescale 1ns / 1ps

module led_decoration_system #(parameter CLOCK_LIMIT = 25000000) (
    input clk,
    input reset_n,
    input [1:0] sw,
    output reg [3:0] seg0_bcd,
    output reg [3:0] seg1_bcd,
    output reg [3:0] seg2_bcd,
    output reg [3:0] seg3_bcd
);

    wire rst = reset_n;

    localparam NUM_2 = 4'd2;
    localparam NUM_5 = 4'd5;
    localparam BLANK = 4'hF;

    reg [25:0] counter;
    reg move_tick;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            counter <= 0;
            move_tick <= 0;
        end else begin
            if (counter >= CLOCK_LIMIT - 1) begin
                counter <= 0;
                move_tick <= 1;
            end else begin
                counter <= counter + 1;
                move_tick <= 0;
            end
        end
    end

    reg [1:0] step;
    reg direction;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            step <= 0;
            direction <= 0;
        end else if (move_tick) begin
            case (sw)
                2'b00: begin
                    step <= step - 1;
                end
                2'b01: begin
                    step <= step + 1;
                end
                2'b10: begin
                    if (direction == 0) begin
                        if (step == 2) begin
                            direction <= 1;
                            step <= 1;
                        end else begin
                            step <= step + 1;
                        end
                    end else begin
                        if (step == 0) begin
                            direction <= 0;
                            step <= 1;
                        end else begin
                            step <= step - 1;
                        end
                    end
                end
                default: step <= 0;
            endcase
        end
    end

    always @(*) begin
        seg3_bcd = BLANK; seg2_bcd = BLANK; seg1_bcd = BLANK; seg0_bcd = BLANK;

        case (step)
            0: begin
                seg3_bcd = NUM_2; seg2_bcd = NUM_5;
            end
            1: begin
                seg2_bcd = NUM_2; seg1_bcd = NUM_5;
            end
            2: begin
                seg1_bcd = NUM_2; seg0_bcd = NUM_5;
            end
            3: begin
                seg0_bcd = NUM_2; seg3_bcd = NUM_5;
            end
        endcase
    end

endmodule