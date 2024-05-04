#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "font.h"

#define SSD1306_I2C_CLK 400

void drawChar(unsigned char x, unsigned char y, char letter);
void drawString(unsigned char x, unsigned char y, char *str);



int main() {

    stdio_init_all();   // UART, not needed (?)

    // set up i2c
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);      // unclear if breakout board has pull-ups already
    // gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);      // -- if not, uncomment these lines
    //

    ssd1306_setup();    // set up ssd1306 OLED display

    // heartbeat
    unsigned char heartbeat_x, heartbeat_y;
    heartbeat_x = 127;
    heartbeat_y = 0;
    //

    // adc setup
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    //

    // heartbeat
    ssd1306_drawPixel(heartbeat_x, heartbeat_y, 1);
    ssd1306_update();
    sleep_ms(500);
    ssd1306_drawPixel(heartbeat_x, heartbeat_y, 0);
    ssd1306_update();
    //
    int t1, t2;
    t1 = 1;

    while (1) {
        
        t2 = to_us_since_boot(get_absolute_time());     // fetch current time

        // get adc value and print to screen
        uint16_t adc_val = adc_read();
        char msg[50];
        sprintf(msg, "ADC value: %d ", adc_val);    // extra space after makes sure no old digits hang off end
        drawString(0, 0, msg);
        //

        sprintf(msg, "%f", 1000000.0/(t2-t1));
        drawString(0, 24, msg);

        ssd1306_update();       // update screen
        t1 = t2;
    }

    return (0);
}



void drawChar(unsigned char x, unsigned char y, char letter) {

    // have to shift letter by 0x20 based on how the array ASCII is laid out
    for (char i=0; i<5; i++) {                          // loop through each col
        for (int j=0; j<8; j++) {                       // loop through each row
            ssd1306_drawPixel(x+i, y+j, \
            ((ASCII[letter-0x20][i] & (0b1<<j)) >> j)); // determine 1 or 0 with col mask and shift
        }
    }
}

void drawString(unsigned char x, unsigned char y, char *str) {

    while (*str) {
        drawChar(x, y, *str++);
        x += 6;
    }
}