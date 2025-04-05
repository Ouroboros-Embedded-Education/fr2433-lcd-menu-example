/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "gpio.h"
#include "lcdDisplay.h"

#include "driverlib.h"
#include "Board.h"

/* LCD Driver Vars and Functions */

lcd_t Lcd = {0};


/* Peripheral Initializations */

void _lcd_start(){
    Lcd.gpios[LCD_RS].GPIO = LCD_RS_GPIO;
    Lcd.gpios[LCD_RS].pin = LCD_RS_PIN;
    Lcd.gpios[LCD_E].GPIO = LCD_E_GPIO;
    Lcd.gpios[LCD_E].pin = LCD_E_PIN;
    Lcd.gpios[LCD_D4].GPIO = LCD_D4_GPIO;
    Lcd.gpios[LCD_D4].pin = LCD_D4_PIN;
    Lcd.gpios[LCD_D5].GPIO = LCD_D5_GPIO;
    Lcd.gpios[LCD_D5].pin = LCD_D5_PIN;
    Lcd.gpios[LCD_D6].GPIO = LCD_D6_GPIO;
    Lcd.gpios[LCD_D6].pin = LCD_D6_PIN;
    Lcd.gpios[LCD_D7].GPIO = LCD_D7_GPIO;
    Lcd.gpios[LCD_D7].pin = LCD_D7_PIN;
    Lcd.columns = 20;
    Lcd.rows = 4;
    Lcd.interface = LCD_INTERFACE_4BIT;       
    
    lcd_init(&Lcd);
}

void main (void)
{
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //Initialize the clock
    board_clock_setup();

    //Initialize gpios 
    board_gpio_setup();
    GPIO_setOutputHighOnPin(USER_LED2_GPIO, USER_LED2_PIN);

    //Start display LCD
    _lcd_start();

    GPIO_setOutputLowOnPin(USER_LED2_GPIO, USER_LED2_PIN);
    lcd_send_string_pos(&Lcd, "Rusbe", 1 , 1);

    while (true){
        GPIO_toggleOutputOnPin(USER_LED1_GPIO, USER_LED1_PIN);
        delay_ms(50);
    }
}
