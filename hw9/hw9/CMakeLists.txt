add_executable(hw9)

target_sources(hw9 PRIVATE hw9.c)

target_link_libraries(hw9 pico_stdlib hardware_gpio hardware_irq)
pico_enable_stdio_usb(hw9 1)
pico_enable_stdio_uart(hw9 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw9)