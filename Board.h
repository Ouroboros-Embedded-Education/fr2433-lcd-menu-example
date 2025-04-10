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
/**
 * ===
 * Board.h
 * ===
 * This file is meant to provide generic GPIO selection definitions that allows
 * DriverLib to be used portably across different LaunchPads. The same module
 * pin might not be populated on the same physical port and pin on different
 * LaunchPads.
 *
 * This file is intended to be modified and updated by individual users based
 * on application need. It will be initially populated according to common
 * peripherals available on a LaunchPad board, but by all means not complete.
 *
 * GPIO_PRIMARY_MODULE_FUNCTION indicates Port SEL bits are 01
 * GPIO_SECONDARY_MODULE_FUNCTION indicates Port SEL bits are 10
 * GPIO_TERNARY_MODULE_FUNCTION indicates Port SEL bits are 11
 * If the Port SEL bits are 00, the FUNCTION bits should be ignored
 * Please consult device datasheet for more info, under "Input/Output Diagrams"
 *
 **/

#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>

#include "driverlib.h"


#define LCD_RS_GPIO         GPIO_PORT_P1
#define LCD_RS_PIN          GPIO_PIN5

#define LCD_E_GPIO          GPIO_PORT_P1
#define LCD_E_PIN           GPIO_PIN4

#define LCD_D4_GPIO         GPIO_PORT_P1
#define LCD_D4_PIN          GPIO_PIN6

#define LCD_D5_GPIO         GPIO_PORT_P1
#define LCD_D5_PIN          GPIO_PIN7

#define LCD_D6_GPIO         GPIO_PORT_P2
#define LCD_D6_PIN          GPIO_PIN4

#define LCD_D7_GPIO         GPIO_PORT_P2
#define LCD_D7_PIN          GPIO_PIN7

#define USER_BTN1_GPIO      GPIO_PORT_P1
#define USER_BTN1_PIN       GPIO_PIN3

#define USER_BTN2_GPIO      GPIO_PORT_P1
#define USER_BTN2_PIN       GPIO_PIN2

#define USER_LED1_GPIO      GPIO_PORT_P1
#define USER_LED1_PIN       GPIO_PIN1

#define USER_LED2_GPIO      GPIO_PORT_P1
#define USER_LED2_PIN       GPIO_PIN0

//*****************************************************************************
//
//Target frequency for MCLK in kHz
//
//*****************************************************************************
#define CS_MCLK_DESIRED_FREQUENCY_IN_KHZ   1000

//*****************************************************************************
//
//MCLK/FLLRef Ratio
//
//*****************************************************************************
#define CS_MCLK_FLLREF_RATIO   30

/* Publics */

void board_gpio_setup();

void board_clock_setup();

void delay_us(int32_t us);

void delay_ms(uint32_t ms);

#endif // __BOARD_H__

