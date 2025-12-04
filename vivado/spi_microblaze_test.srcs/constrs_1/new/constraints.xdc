set_property IOSTANDARD LVDS [get_ports sys_clk_clk_p]
set_property IOSTANDARD LVDS [get_ports sys_clk_clk_n]
set_property PACKAGE_PIN T24 [get_ports sys_clk_clk_p]

set_property IOSTANDARD LVCMOS33 [get_ports uart_*]
set_property PACKAGE_PIN A12 [get_ports uart_rxd]
set_property PACKAGE_PIN A13 [get_ports uart_txd]

set_property IOSTANDARD LVCMOS33 [get_ports spi_*]
set_property PACKAGE_PIN AC14 [get_ports spi_clk]
set_property PACKAGE_PIN AC13 [get_ports spi_mosi]
set_property PACKAGE_PIN AA13 [get_ports {spi_cs[0]}]

set_property IOSTANDARD LVCMOS12 [get_ports resetn]
set_property PACKAGE_PIN N26 [get_ports resetn]

set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets clk]
