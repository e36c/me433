add_executable(hw12)

target_sources(hw12 PRIVATE hw12.c)

target_link_libraries(hw12 pico_stdlib hardware_gpio hardware_pwm)
pico_enable_stdio_usb(hw12 1)
pico_enable_stdio_uart(hw12 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw12)