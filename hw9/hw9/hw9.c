#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/irq.h"

// UART defines
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0   // default UART pins
#define UART_RX_PIN 1
//


volatile int chars_rxed = 0;
char message[100];      // global for int received from computer
int rxed_int;


void on_uart_rx();


int main() {

    stdio_init_all();

    // UART setup **********************************
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 115200);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int actual = uart_set_baudrate(UART_ID, BAUD_RATE);

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

    // ****************************** UART all set up.


    // wait for USB enumeration
    while (!stdio_usb_connected()) { sleep_ms(100); }   // wait for USB to enumerate Pico
    printf("\r\nActual baud: %d", actual);
    //

    // variables used in loop
    char msg[100];              // user input string
    int input;                  // user input number -- max of 100
    //

    while (1) {
        
        // take user input
        printf("\r\nEnter number: ", msg);
        scanf("%d", &input);                        // watch out for this blocking the interrupt -- unlikely but possible
        sprintf(msg, "%d", input);
        uart_puts(uart0, msg);                    // send to Zero
        //
    }

    return (0);
}


// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        unsigned char ch = uart_getc(UART_ID);
        //printf("\r\nChar rx'd: %c", ch);

        if (ch == '\n') {

            sscanf(message, "%d", &rxed_int);
            printf("\r\nInt rx'd from Zero: %d\r\n", rxed_int);
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