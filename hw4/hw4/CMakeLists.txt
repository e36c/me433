add_executable(hw4)

target_sources(hw4 PRIVATE hw4.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(hw4 pico_stdlib hardware_spi hardware_gpio pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw4)

