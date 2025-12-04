//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2024.2 (win64) Build 5239630 Fri Nov 08 22:35:27 MST 2024
//Date        : Wed Dec  3 23:49:44 2025
//Host        : DESKTOP-9384M68 running 64-bit major release  (build 9200)
//Command     : generate_target design_1_wrapper.bd
//Design      : design_1_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module design_1_wrapper
   (resetn,
    spi_clk,
    spi_cs,
    spi_mosi,
    sys_clk_clk_n,
    sys_clk_clk_p,
    uart_rxd,
    uart_txd);
  input resetn;
  output spi_clk;
  output [0:0]spi_cs;
  output spi_mosi;
  input sys_clk_clk_n;
  input sys_clk_clk_p;
  input uart_rxd;
  output uart_txd;

  wire resetn;
  wire spi_clk;
  wire [0:0]spi_cs;
  wire spi_mosi;
  wire sys_clk_clk_n;
  wire sys_clk_clk_p;
  wire uart_rxd;
  wire uart_txd;

  design_1 design_1_i
       (.resetn(resetn),
        .spi_clk(spi_clk),
        .spi_cs(spi_cs),
        .spi_mosi(spi_mosi),
        .sys_clk_clk_n(sys_clk_clk_n),
        .sys_clk_clk_p(sys_clk_clk_p),
        .uart_rxd(uart_rxd),
        .uart_txd(uart_txd));
endmodule
