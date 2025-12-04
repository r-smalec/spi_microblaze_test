// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
// -------------------------------------------------------------------------------

`timescale 1 ps / 1 ps

(* BLOCK_STUB = "true" *)
module design_1 (
  sys_clk_clk_n,
  sys_clk_clk_p,
  uart_rxd,
  uart_txd,
  resetn,
  spi_clk,
  spi_cs,
  spi_mosi
);

  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 sys_clk CLK_N" *)
  (* X_INTERFACE_MODE = "slave sys_clk" *)
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME sys_clk, CAN_DEBUG false, FREQ_HZ 200000000" *)
  input sys_clk_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 sys_clk CLK_P" *)
  input sys_clk_clk_p;
  (* X_INTERFACE_IGNORE = "true" *)
  input uart_rxd;
  (* X_INTERFACE_IGNORE = "true" *)
  output uart_txd;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.RESETN RST" *)
  (* X_INTERFACE_MODE = "slave RST.RESETN" *)
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.RESETN, POLARITY ACTIVE_LOW, INSERT_VIP 0" *)
  input resetn;
  (* X_INTERFACE_IGNORE = "true" *)
  output spi_clk;
  (* X_INTERFACE_IGNORE = "true" *)
  output [0:0]spi_cs;
  (* X_INTERFACE_IGNORE = "true" *)
  output spi_mosi;

  // stub module has no contents

endmodule
