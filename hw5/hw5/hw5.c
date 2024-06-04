#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define MCP23008_I2C_ADDR   0b0100000   // device address
#define IODIR_ADDR          0x00
#define IOCON_ADDR          0x05
#define GPIO_ADDR           0x09
#define OLAT_ADDR           0x0A

#define I2C_CLK             100         // i2c clock


void mcp23008_setup();


int main() {

    stdio_init_all();   // UART, not needed (?)

    // set up i2c, gpios
    i2c_init(i2c_default, I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    //

    mcp23008_setup();    // set up mcp23008 I/O expander


    uint8_t rx_buf;             // buffer to read data into
    char led_on = 0;            // variable for saving LED state, to invert it

    while (1) {

        // read button
        rx_buf = GPIO_ADDR;
        i2c_write_blocking(i2c_default, MCP23008_I2C_ADDR, &rx_buf, 1, true);    // must write to tell what register we want to read from next!
        i2c_read_blocking(i2c_default, MCP23008_I2C_ADDR, &rx_buf, 1, false);    // we've used rx_buf for both here
        rx_buf = !(rx_buf & 0b1);       // mask with bit 1, where the button is connected wiring active-low wiring (pullup)
        //
        
        sleep_ms(5);

        // write to output register
        uint8_t tx_buf[2];
        tx_buf[0] = OLAT_ADDR;
        tx_buf[1] = (rx_buf*0xff) & 0x80;   // make LED (GP7) 1 if rx_buf is 1
        i2c_write_blocking(i2c_default, MCP23008_I2C_ADDR, tx_buf, 2, false);

        sleep_ms(5);
    }

    return (0);
}



void mcp23008_setup() {

    uint8_t buf[2];
    buf[0] = IODIR_ADDR;    // write to IO direction register
    buf[1] = 0x7f;          // GP7 output, all else inputs (though only GP0 is used)
    i2c_write_blocking(i2c_default, MCP23008_I2C_ADDR, buf, 2, false);

    sleep_ms(10);
    
    // buf[0] = IOCON_ADDR;    // write to IO control register
    // buf[1] = 0x36;          // write 1 to SEQOP (bit 5), other stuff is default (I think)
    // i2c_write_blocking(i2c_default, MCP23008_I2C_ADDR, buf, 2, false);

    sleep_ms(10);
}