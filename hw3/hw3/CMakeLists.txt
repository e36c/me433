add_executable(hw3)

target_sources(hw3 PRIVATE hw3.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(hw3 pico_stdlib hardware_adc hardware_gpio)
pico_enable_stdio_usb(hw3 1)
pico_enable_stdio_uart(hw3 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw3)

