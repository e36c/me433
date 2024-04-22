#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hw4.h"
#include "math.h"

#define LOOP_FREQ 500       // Hz
#define LED_PIN 15


static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}


static void spi_setup() {
    spi_init(spi_default, 5000);                                  // configure SPI at 0.5MHz
    // gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);     // configure SPI SCK pin
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);      // configure SPI TX pin
    
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);                            // initialize CS pin to high (active-low)
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
}


uint16_t mcp4902_spi_generator(float voltage, bool channel) {

    if (voltage>3.3) {voltage = 3.3;}           // saturate voltage if needed
    else if (voltage<0) {voltage = 0;}

    int D_bits = (int) (voltage/3.3 * 4095);    // convert voltage to 12 bit number (works for 8, 10, and 12-bit mc49xx)

    printf("\r\nvoltage = %d V", D_bits);
    
    uint16_t output = D_bits | ((0b0111 | ((channel & 0b1)<<3)) << 12);
        // 0b111 = enable buffer, gain=1, no shutdown
        // ((channel & 0b1)<<3) masks last bit of channel (maybe not necessary) then shifts into channel bit
    
    return output;
}

static inline void spi_write(uint16_t data) {

    unsigned char write_buf[2];              // *********** !!!!!! ORDER CORRECT?????
    write_buf[1] = data & 0b11111111;                       // mask lower byte
    write_buf[0] = (data & (0b11111111<<8)) >> 8;           // mask upper byte

    cs_select();
    spi_write_blocking(spi_default, write_buf, 2);         // last argument allows us to write two bytes with one call
    cs_deselect();
}



int main() {
    stdio_init_all();   // UART, not needed (?)
    spi_setup();        // set up SPI

    float sin_output = 0;                           // declare output voltages. these are updated each loop
    float tri_output = 0;

    volatile float tri_delta = 1.0/LOOP_FREQ*2*3.3 * 4.41;        // slope of triangle wave. last term is fudge factor (probably dependent on loop duration and max output of DAC)

    gpio_init(LED_PIN);                 // GPIO
    gpio_set_dir(LED_PIN, GPIO_OUT);

    int n = 0;
    while (1) {

        // calculate output values
        sin_output = 3.3/2 * (1 + sin(2*3.14 * n/LOOP_FREQ * 9));   // 2Hz sine wave output, last term is fudge factor
        tri_output += tri_delta;                                    // 1Hz triangle wave output
        if (tri_output >= 3.1) { tri_delta = -tri_delta; }
        if (tri_output <= 0)   { tri_delta = -tri_delta; }

        // heartbeat
        gpio_put(LED_PIN, 1);

        spi_write(mcp4902_spi_generator(sin_output,0));     // write sine
        spi_write(mcp4902_spi_generator(tri_output,1));     // write triangle

        n++;
        if (n>LOOP_FREQ) { n=0; }                           // rollover n

        sleep_ms(1000/LOOP_FREQ);                           // loops at [LOOP_FREQ]Hz
    }
}
