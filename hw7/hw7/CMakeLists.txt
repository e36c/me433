add_executable(hw7)

target_sources(hw7 PRIVATE hw7.c ssd1306.c)

target_link_libraries(hw7 pico_stdlib hardware_i2c hardware_uart hardware_gpio hardware_irq)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw7)