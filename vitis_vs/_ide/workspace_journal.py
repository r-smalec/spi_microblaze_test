# 2025-12-04T12:14:41.796427200
import vitis

client = vitis.create_client()
client.set_workspace(path="vitis_vs")

platform = client.create_platform_component(name = "spi_test_platform",hw_design = "$COMPONENT_LOCATION/../../../_FPGA/_PW/spi_microblaze_test/outputs/spi_microblaze_test_3.xsa",os = "standalone",cpu = "microblaze_0",domain_name = "standalone_microblaze_0")

platform = client.get_component(name="spi_test_platform")
status = platform.build()

comp = client.create_app_component(name="spi_test_app",platform = "$COMPONENT_LOCATION/../spi_test_platform/export/spi_test_platform/spi_test_platform.xpfm",domain = "standalone_microblaze_0")

status = platform.build()

comp = client.get_component(name="spi_test_app")
comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

