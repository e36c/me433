#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define PWM_PIN     15
#define CLOCK_DIV   38
#define ROLLOVER    65534


void init_servo_pwm(int pin, int clk_div, int rlvr);
void set_servo_angle(int angle);


int main() {

    stdio_init_all();

    // wait for USB enumeration
    // while (!stdio_usb_connected()) { sleep_ms(100); }   // wait for USB to enumerate Pico
    //

    // initialize servo PWM
    init_servo_pwm(PWM_PIN, CLOCK_DIV, ROLLOVER);

    // variables used in loop
    // char msg[100];              // user input string
    // int input = 0;              // user input number -- max of 100
    //

    while (1) {

        // testing loop that takes user input --------------------
        // set_servo_angle(input);     // set to 0deg initially

        // take user input
        // printf("\r\nEnter angle -90 to 90: ", msg);
        // scanf("%d", &input);
        // printf("\r\nInput rx'd: %d", input);
        // -------------------------------------------------------

        for (int i=-90; i<=90; i++) {
            set_servo_angle(i);
            sleep_ms(2000/180);
        }
        for (int i=90; i>=-90; i--) {
            set_servo_angle(i);
            sleep_ms(2000/180);
        }       

    }

    return (0);
}



void init_servo_pwm(int pin, int clk_div, int rlvr) {

    // set output pin for servo control
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    //

    // set up PWM for 50Hz
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);    // find PWM pin slice
    pwm_set_clkdiv(slice_num, clk_div);                 // clock divider; arg 2 is float be between 1-255
    pwm_set_chan_level(slice_num, PWM_PIN, 1);          // set channel A output high for one cycle before dropping
    pwm_set_wrap(slice_num, rlvr);                      // when to rollover; arg 2 must be < 65535
    pwm_set_enabled(slice_num, true);                   // enable PWM
    //
}


void set_servo_angle(int angle) {

    // saturate angle at servo limits
    if (angle<-90)      angle=-90;
    else if (angle>90)  angle=90;
    //

    int duty = ROLLOVER/100. * (7.5 + angle*(5./90));
    pwm_set_gpio_level(PWM_PIN, duty);

}