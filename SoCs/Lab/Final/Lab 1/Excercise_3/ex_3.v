module led_string_system #(
    parameter CLOCK_FREQ = 125000000,
    parameter BAUD_RATE  = 9600
)(
    input clk,
    input reset_n,
    input [3:1] btn,
    output reg [3:0] led,
    output uart_tx_out
);

    localparam CLOCK_LIMIT = CLOCK_FREQ - 1;

    reg [1:0] mode;

    always @(posedge clk or posedge reset_n) begin
        if (reset_n) begin
            mode <= 0;
        end else begin
            if (btn[3])      mode <= 0;
            else if (btn[1]) mode <= 1;
            else if (btn[2]) mode <= 2;
        end
    end

    reg [31:0] counter;
    reg tick_1hz;

    always @(posedge clk or posedge reset_n) begin
        if (reset_n) begin
            counter <= 0;
            tick_1hz <= 0;
        end else begin
            if (counter >= CLOCK_LIMIT) begin
                counter <= 0;
                tick_1hz <= 1;
            end else begin
                counter <= counter + 1;
                tick_1hz <= 0;
            end
        end
    end

    always @(posedge clk or posedge reset_n) begin
        if (reset_n) begin
            led <= 4'b0011;
        end else if (tick_1hz) begin
            case (mode)
                1: led <= {led[2:0], led[3]};
                2: led <= {led[0], led[3:1]};
                default: led <= led;
            endcase
        end
    end

    reg uart_start;
    reg [7:0] uart_data;
    wire uart_busy;

    uart_tx #(
        .CLOCK_FREQ(CLOCK_FREQ),
        .BAUD_RATE(BAUD_RATE)
    ) u_tx (
        .clk(clk),
        .rst(reset_n),
        .start(uart_start),
        .data(uart_data),
        .tx(uart_tx_out),
        .busy(uart_busy)
    );

    reg [3:0] tx_state;
    reg [3:0] led_snapshot;

    localparam CHAR_0 = 8'h30;
    localparam CHAR_1 = 8'h31;
    localparam CHAR_LF = 8'h0A;

    always @(posedge clk or posedge reset_n) begin
        if (reset_n) begin
            tx_state <= 0;
            uart_start <= 0;
            uart_data <= 0;
        end else begin
            case (tx_state)
                0: begin
                    uart_start <= 0;
                    if (tick_1hz) begin
                        led_snapshot <= led;
                        tx_state <= 1;
                    end
                end

                1: begin
                    if (!uart_busy) begin
                        uart_data <= (led_snapshot[3]) ? CHAR_1 : CHAR_0;
                        uart_start <= 1;
                        tx_state <= 2;
                    end
                end
                2: begin uart_start <= 0; tx_state <= 3; end

                3: begin
                    if (!uart_busy) begin
                        uart_data <= (led_snapshot[2]) ? CHAR_1 : CHAR_0;
                        uart_start <= 1;
                        tx_state <= 4;
                    end
                end
                4: begin uart_start <= 0; tx_state <= 5; end

                5: begin
                    if (!uart_busy) begin
                        uart_data <= (led_snapshot[1]) ? CHAR_1 : CHAR_0;
                        uart_start <= 1;
                        tx_state <= 6;
                    end
                end
                6: begin uart_start <= 0; tx_state <= 7; end

                7: begin
                    if (!uart_busy) begin
                        uart_data <= (led_snapshot[0]) ? CHAR_1 : CHAR_0;
                        uart_start <= 1;
                        tx_state <= 8;
                    end
                end
                8: begin uart_start <= 0; tx_state <= 9; end

                9: begin
                    if (!uart_busy) begin
                        uart_data <= CHAR_LF;
                        uart_start <= 1;
                        tx_state <= 10;
                    end
                end
                10: begin
                    uart_start <= 0;
                    tx_state <= 11;
                end

                11: begin
                    if (!uart_busy) tx_state <= 0;
                end

            endcase
        end
    end
endmodule