`timescale 1ns / 1ps

/**
 * @param a first 1-bit input
 * @param b second 1-bit input
 * @param g whether a and b generate a carry
 * @param p whether a and b would propagate an incoming carry
 */
module gp1(input wire a, b,
           output wire g, p);
   assign g = a & b;
   assign p = a | b;
endmodule

/**
 * Computes aggregate generate/propagate signals over a 4-bit window.
 * @param gin incoming generate signals
 * @param pin incoming propagate signals
 * @param cin the incoming carry
 * @param gout whether these 4 bits internally would generate a carry-out (independent of cin)
 * @param pout whether these 4 bits internally would propagate an incoming carry from cin
 * @param cout the carry outs for the low-order 3 bits
 */
module gp4(input wire [3:0] gin, pin,
           input wire cin,
           output wire gout, pout,
           output wire [2:0] cout);

   assign cout[0] = gin[0] | (pin[0] & cin);
   assign cout[1] = gin[1] | (pin[1] & gin[0]) | (pin[1] & pin[0] & cin);
   assign cout[2] = gin[2] | (pin[2] & gin[1]) | (pin[2] & pin[1] & gin[0]) | (pin[2] & pin[1] & pin[0] & cin);

   assign pout = pin[3] & pin[2] & pin[1] & pin[0];
   assign gout = gin[3] | (pin[3] & gin[2]) | (pin[3] & pin[2] & gin[1]) | (pin[3] & pin[2] & pin[1] & gin[0]);

endmodule

/** Same as gp4 but for an 8-bit window instead */
module gp8(input wire [7:0] gin, pin,
           input wire cin,
           output wire gout, pout,
           output wire [6:0] cout);
   
   wire g_low, p_low, g_high, p_high;
   wire c_mid;
   wire [2:0] cout_low;
   wire [2:0] cout_high;

   gp4 low_nibble (.gin(gin[3:0]), .pin(pin[3:0]), .cin(cin), 
                   .gout(g_low), .pout(p_low), .cout(cout_low));

   assign c_mid = g_low | (p_low & cin);

   gp4 high_nibble (.gin(gin[7:4]), .pin(pin[7:4]), .cin(c_mid), 
                    .gout(g_high), .pout(p_high), .cout(cout_high));

   assign pout = p_high & p_low;
   assign gout = g_high | (p_high & g_low);
   assign cout = {cout_high, c_mid, cout_low};

endmodule

module cla
  (input wire [31:0]  a, b,
   input wire         cin,
   output wire [31:0] sum);

   wire [31:0] g, p;
   wire [3:0]  G, P;
   wire [3:0]  C_blk;
   wire [6:0]  C_int [3:0];

   genvar i;
   generate
      for (i = 0; i < 32; i = i + 1) begin : bit_gp
         gp1 unit (.a(a[i]), .b(b[i]), .g(g[i]), .p(p[i]));
      end
   endgenerate

   assign C_blk[0] = cin;
   assign C_blk[1] = G[0] | (P[0] & C_blk[0]);
   assign C_blk[2] = G[1] | (P[1] & G[0]) | (P[1] & P[0] & C_blk[0]);
   assign C_blk[3] = G[2] | (P[2] & G[1]) | (P[2] & P[1] & G[0]) | (P[2] & P[1] & P[0] & C_blk[0]);
   
   gp8 block0 (.gin(g[7:0]),   .pin(p[7:0]),   .cin(C_blk[0]), .gout(G[0]), .pout(P[0]), .cout(C_int[0]));
   gp8 block1 (.gin(g[15:8]),  .pin(p[15:8]),  .cin(C_blk[1]), .gout(G[1]), .pout(P[1]), .cout(C_int[1]));
   gp8 block2 (.gin(g[23:16]), .pin(p[23:16]), .cin(C_blk[2]), .gout(G[2]), .pout(P[2]), .cout(C_int[2]));
   gp8 block3 (.gin(g[31:24]), .pin(p[31:24]), .cin(C_blk[3]), .gout(G[3]), .pout(P[3]), .cout(C_int[3]));

   generate
      for (i = 0; i < 4; i = i + 1) begin : sum_calc
         assign sum[i*8] = (p[i*8] & ~g[i*8]) ^ C_blk[i];
         
         assign sum[i*8+1] = (p[i*8+1] & ~g[i*8+1]) ^ C_int[i][0];
         assign sum[i*8+2] = (p[i*8+2] & ~g[i*8+2]) ^ C_int[i][1];
         assign sum[i*8+3] = (p[i*8+3] & ~g[i*8+3]) ^ C_int[i][2];
         assign sum[i*8+4] = (p[i*8+4] & ~g[i*8+4]) ^ C_int[i][3];
         assign sum[i*8+5] = (p[i*8+5] & ~g[i*8+5]) ^ C_int[i][4];
         assign sum[i*8+6] = (p[i*8+6] & ~g[i*8+6]) ^ C_int[i][5];
         assign sum[i*8+7] = (p[i*8+7] & ~g[i*8+7]) ^ C_int[i][6];
      end
   endgenerate
endmodule