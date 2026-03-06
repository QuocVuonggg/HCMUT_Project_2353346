module system (
    input         clk,
    input  [0:0]  btn,
    output [3:0]  ck_io,
    output        halt_led
);

    wire [31:0] cpu_debug_data;
    wire processor_halt;
    
    Processor my_cpu (
        .clock_proc   (clk),
        .clock_mem    (~clk),
        .rst          (btn[0]),
        .halt         (processor_halt),
        .led_bcd      (cpu_debug_data)
    );

    assign ck_io = cpu_debug_data[3:0];
    assign halt_led = processor_halt;

endmodule