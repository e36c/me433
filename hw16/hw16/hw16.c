#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define DIRECTION_PIN       14
#define M1_SPEED_PIN        15


int main() {

    stdio_init_all();

    // wait for USB enumeration
    while (!stdio_usb_connected()) { sleep_ms(100); }   // wait for USB to enumerate Pico
    //

    // set output pins for H-bridge control
    gpio_init(DIRECTION_PIN);
    gpio_set_dir(DIRECTION_PIN, GPIO_OUT);
    gpio_set_function(M1_SPEED_PIN, GPIO_FUNC_PWM);
    //

    // set up PWM
    uint slice_num = pwm_gpio_to_slice_num(M1_SPEED_PIN);   // find PWM pin slice
    pwm_set_wrap(slice_num, 65535);
    pwm_set_chan_level(slice_num, M1_SPEED_PIN, 1);         // Set channel A output high for one cycle before dropping
    pwm_set_enabled(slice_num, true);                       // enable PWM
    //

    // variables used in loop
    char msg[100];              // user input string
    int input;                  // user input number -- max of 100
    int duty = 50;              // PWM duty cycle
    int sat_duty = 25;          // point at which we saturate to 100% duty cycle
    //

    while (1) {
        
        // take user input
        printf("\r\nEnter number 0-100: ", msg);
        scanf("%d", &input);
        printf("\r\nInput rx'd: %d", input);
        //

        // map input onto duty cycle
        if (input<sat_duty)     duty = 65535*input/sat_duty;
        else                    duty = 65535;
        //

        // change duty cycle and report back to user
        pwm_set_gpio_level(M1_SPEED_PIN, duty);
        printf("\r\nDuty cycle out of 65535: %d", duty);
        //
    }

    return (0);
}