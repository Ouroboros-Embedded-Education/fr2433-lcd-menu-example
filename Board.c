#include "Board.h"
#include "gpio.h"

void board_gpio_init(){
    /* Configure LCD GPIO output pins */
    GPIO_setAsOutputPin(LCD_RS_GPIO, LCD_RS_PIN);
    GPIO_setAsOutputPin(LCD_E_GPIO, LCD_E_PIN);
    GPIO_setAsOutputPin(LCD_D4_GPIO, LCD_D4_PIN);
    GPIO_setAsOutputPin(LCD_D5_GPIO, LCD_D5_PIN);
    GPIO_setAsOutputPin(LCD_D6_GPIO, LCD_D6_PIN);
    GPIO_setAsOutputPin(LCD_D7_GPIO, LCD_D7_PIN);

    GPIO_setOutputLowOnPin(LCD_RS_GPIO, LCD_RS_PIN);
    GPIO_setOutputLowOnPin(LCD_E_GPIO, LCD_E_PIN);
    GPIO_setOutputLowOnPin(LCD_D4_GPIO, LCD_D4_PIN);
    GPIO_setOutputLowOnPin(LCD_D5_GPIO, LCD_D5_PIN);
    GPIO_setOutputLowOnPin(LCD_D6_GPIO, LCD_D6_PIN);
    GPIO_setOutputLowOnPin(LCD_D7_GPIO, LCD_D7_PIN);

    /* Configure the LEDs */
    GPIO_setAsOutputPin(USER_LED1_GPIO, USER_LED1_PIN);
    GPIO_setAsOutputPin(USER_LED2_GPIO, USER_LED2_PIN);

    GPIO_setOutputLowOnPin(USER_LED1_GPIO, USER_LED1_PIN);
    GPIO_setOutputLowOnPin(USER_LED2_GPIO, USER_LED2_PIN);

    /* Configure Button Inputs */
    GPIO_setAsInputPinWithPullUpResistor(USER_BTN1_GPIO, USER_BTN1_PIN);
    GPIO_setAsInputPinWithPullUpResistor(USER_BTN2_GPIO, USER_BTN2_PIN);

    /* Enable the GPIOs */
    PMM_unlockLPM5();
}
