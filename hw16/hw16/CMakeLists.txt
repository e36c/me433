add_executable(hw16)

target_sources(hw16 PRIVATE hw16.c)

target_link_libraries(hw16 pico_stdlib hardware_gpio hardware_pwm)
pico_enable_stdio_usb(hw16 1)
pico_enable_stdio_uart(hw16 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw16)