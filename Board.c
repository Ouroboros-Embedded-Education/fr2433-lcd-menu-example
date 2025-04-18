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
    GPIO_setAsInputPinWithPullUpResistor(USER_SELECT_GPIO, USER_SELECT_PIN);
    GPIO_setAsInputPinWithPullUpResistor(USER_ENTER_GPIO, USER_ENTER_PIN);
            
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

void board_timer_setup(){
    //Start timer in continuous mode sourced by SMCLK (same as MCLK 1MHz)
    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_A_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

    //Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0
        );
		
    // Configure compare to generate interrupts every 1ms
    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue = 999;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);


    Timer_A_startCounter( TIMER_A1_BASE,
            TIMER_A_CONTINUOUS_MODE
                );

    // enable interrupts
    __bis_SR_register(GIE);

}

void delay_us(int32_t us){
    // TODO use a timer instead this
    do {
        __delay_cycles(5);
        us -= 200;
    } while (us > 0);
}

void delay_ms(uint32_t ms){
    while (ms > 0){
        delay_us(1000);
        ms--;
    }
}

void __attribute__((__weak__)) board_1ms_interrupt(){


}

//******************************************************************************
//
//This is the TIMER1_A0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER1_A0_ISR (void)
{
    uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0)
            + 999;

    board_1ms_interrupt();

    //Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}
