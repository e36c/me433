add_executable(hw6)

target_sources(hw6 PRIVATE hw6.c ssd1306.c)

target_link_libraries(hw6 pico_stdlib hardware_i2c hardware_gpio hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw6)