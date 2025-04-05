#include "Board.h"
#include "gpio.h"
#include "intrinsics.h"

void board_gpio_setup(){
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
            
    /*
    //MCLK set out to pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN3,
        GPIO_SECONDARY_MODULE_FUNCTION
        );*/

    /* Enable the GPIOs */
    PMM_unlockLPM5();
}

void board_clock_setup(){
    //Set DCO FLL reference = REFO
    CS_initClockSignal(
        CS_FLLREF,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    //Set ACLK = REFO
    CS_initClockSignal(
        CS_ACLK,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );
    
    //Create struct variable to store proper software trim values
    CS_initFLLParam param = {0};
    
    //Set Ratio/Desired MCLK Frequency, initialize DCO, save trim values
    CS_initFLLCalculateTrim(
        CS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
        CS_MCLK_FLLREF_RATIO,
        &param
        );

    //Clear all OSC fault flag
    CS_clearAllOscFlagsWithTimeout(1000);

    //Enable oscillator fault interrupt
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
}

void delay_us(int32_t us){
    // TODO use a timer instead this
    do {
        __delay_cycles(10);
        us -= 10;
    } while (us > 0);
}

void delay_ms(uint32_t ms){
    while (ms > 0){
        delay_us(1000);
        ms--;
    }
}
