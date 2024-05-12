#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "ssd1306.h"
#include "font.h"

// OLED defines
#define SSD1306_I2C_CLK 400
//

// UART defines
#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0   // default UART pins
#define UART_RX_PIN 1
//



volatile int chars_rxed = 0;
char message[100];

void drawChar(unsigned char x, unsigned char y, char letter);
void drawString(unsigned char x, unsigned char y, char *str);
void on_uart_rx();

int main() {

    stdio_init_all();

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 9600);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // OK, all set up.

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
        
        // type here
        tight_loop_contents();
        // ssd1306_update();       // update screen
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

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        unsigned char ch = uart_getc(UART_ID);

        if (ch == '\r') {                   // if 'enter' key is pressed
            ssd1306_clear();
            drawString(0,0,message);
            ssd1306_update();

            chars_rxed = 0;                 // reset message index

            for (int j=0; j<100; j++) {     // clear message
                message[j] = '\0';
            }
        }
        else {                              // if not 'enter', put letter in message array
            message[chars_rxed] = ch;
            chars_rxed++;                   // increment message index
        }        
    }
}