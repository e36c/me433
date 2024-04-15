#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"

// The LED is connected to GPIO 25
#define LED_PIN 15
#define BTN_PIN 16
#define ADC_PIN 26


int main() {    // Main (runs on core 0)

    // Initialize peripherals
    gpio_init(LED_PIN);                 // GPIO
    gpio_init(BTN_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(BTN_PIN, GPIO_IN);

    stdio_init_all();                   // UART

    adc_init();                         // ADC
    adc_gpio_init(26);                      // use ADC on GPIO 26
    adc_select_input(0);                    // dir = read
    //
    
    while (!stdio_usb_connected()) { sleep_ms(100); }   // wait for USB to enumerate Pico
    printf("\r\nStart!\r\n");


    // main loop
    while (true) {
        gpio_put(LED_PIN, 1);                       // turn on LED
        while (!gpio_get(BTN_PIN)) { sleep_ms(10); } // wait for button press
        gpio_put(LED_PIN, 0);                       // turn off LED

        char message[100];                          // ask user for input
        int loops;
        printf("\r\nEnter number 1-100: ", message);
        scanf("%d", &loops);

        for (loops>0; loops--;) {                    // read and print ADC value
           uint16_t result = adc_read();
           printf("\r\nADC value: %f V", (float) result*3.3/4096);
           sleep_ms(10);                           // read at 100Hz
        }
    }
}
