add_executable(hw5)

target_sources(hw5 PRIVATE hw5.c)

target_link_libraries(hw5 pico_stdlib hardware_i2c hardware_gpio)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw5)