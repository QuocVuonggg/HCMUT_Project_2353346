module SystemTop (
    input  wire       clk,      // Clock từ board (125MHz/100MHz)
    input  wire [0:0] btn,      // Nút nhấn dùng làm Reset
    output wire [0:0] led       // Đèn LED báo hiệu Halt
);

    wire clock_proc;
    wire clock_mem;
    wire rst;
    wire halt;

    // Gán nút nhấn cho Reset
    assign rst = btn[0];
    // Gán tín hiệu Halt ra đèn LED
    assign led[0] = halt;

    // --- Xử lý Clock (Đơn giản hóa cho bài tập) ---
    // Cách chuẩn: Dùng Clocking Wizard IP để tạo phase shift 90 độ.
    // Cách "mì ăn liền" (để chạy được bitstream ngay): 
    // Dùng clock gốc cho proc và đảo clock cho mem (lệch 180 độ thay vì 90).
    // Vivado có thể báo warning về timing nhưng sẽ chạy được logic.
    
    assign clock_proc = clk;
    assign clock_mem  = ~clk; // Giả lập lệch pha

    // Gọi module Processor của bạn
    Processor cpu_inst (
        .clock_proc(clock_proc),
        .clock_mem (clock_mem),
        .rst       (rst),
        .halt      (halt)
    );

endmodule