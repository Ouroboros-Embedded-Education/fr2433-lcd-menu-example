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
#include <string.h>

#include "gpio.h"
#include "lcdDisplay.h"

#include "driverlib.h"
#include "Board.h"

/* LCD Driver Vars and Functions */

lcd_t Lcd = {0};

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

/* Auxiliary functions */

void _led1_turnon(){
    GPIO_setOutputHighOnPin(USER_LED1_GPIO, USER_LED1_PIN);
}

void _led1_turnoff(){
    GPIO_setOutputLowOnPin(USER_LED1_GPIO, USER_LED1_PIN);
}

void _led2_turnon(){
    GPIO_setOutputHighOnPin(USER_LED2_GPIO, USER_LED2_PIN);
}

void _led2_turnoff(){
    GPIO_setOutputLowOnPin(USER_LED2_GPIO, USER_LED2_PIN);
}

/* Menu Directives */

#define MENU_BTN_NONE           0
#define MENU_BTN_ENTER          1
#define MENU_BTN_SELECT         2

typedef enum{
    MENU_SCREEN_MAIN,
    MENU_SCREEN_CONFIG,
    MENU_SCREEN_INFO,
    MENU_SCREEN_RESET
}menu_screen_e;

typedef struct{
    menu_screen_e eScreen;
    uint8_t u8Index;
    bool bRefresh;

    struct {
        bool Led1;
        bool Led2;
    } config;

    struct {
        int Cnt;
    } info;
}menu_t;

menu_t Menu;

/* Menu Routines */

void _menu_screen_main(uint8_t ButtonPressed){
    uint8_t i;
    char *header = "== Main Menu ==";
    char *items[] = {"Configuration",
                     "Information",
                     "Reset"
    };

    switch (ButtonPressed) {
    case MENU_BTN_NONE:
        if (Menu.bRefresh == false){
            return;
        }
        Menu.bRefresh = false;
        break;
    case MENU_BTN_ENTER:
        switch (Menu.u8Index){
        case 0:
            Menu.eScreen = MENU_SCREEN_CONFIG;
            break;
        case 1:
            Menu.eScreen = MENU_SCREEN_INFO;
            break;
        case 2:
            Menu.eScreen = MENU_SCREEN_RESET;
            break;
        }
        Menu.u8Index = 0;
        Menu.bRefresh = true;
        lcd_clear_all(&Lcd);
        return;
    case MENU_BTN_SELECT:
        Menu.u8Index++;
        if (Menu.u8Index > 2){
            Menu.u8Index = 0;
        }
        break;
    }

    lcd_clear_row(&Lcd, 0);
    lcd_send_string_pos(&Lcd, header, 0, 0);
    for (i=0 ; i<3 ; i++){
        lcd_clear_row(&Lcd, (i+1));
        if (Menu.u8Index == i){
            lcd_send_char_pos(&Lcd, '>', (i+1), 0);
        }
        lcd_send_string_pos(&Lcd, items[i], (i+1), 2);
    }

}

void _menu_screen_config(ButtonPressed){
    uint8_t i;
    char *header = "== Configuration ==";
    char *items[] = {"LED1",
                     "LED2",
                     "<<- Return"
    };

    switch (ButtonPressed) {
    case MENU_BTN_NONE:
        if (Menu.bRefresh == false){
            return;
        }
        Menu.bRefresh = false;
        break;
    case MENU_BTN_ENTER:
        switch (Menu.u8Index){
        case 0:
            Menu.config.Led1 = !Menu.config.Led1;
            if (Menu.config.Led1 == true){
                _led1_turnon();
            }
            else{
                _led1_turnoff();
            }
            break;
        case 1:
            Menu.config.Led2 = !Menu.config.Led2;
            if (Menu.config.Led2 == true){
                _led2_turnon();
            }
            else{
                _led2_turnoff();
            }
            break;
        case 2:
            Menu.eScreen = MENU_SCREEN_MAIN;
            Menu.u8Index = 0;
            Menu.bRefresh = true;
            return;
        }
        break;
    case MENU_BTN_SELECT:
        Menu.u8Index++;
        if (Menu.u8Index > 2){
            Menu.u8Index = 0;
        }
        break;
    }

    lcd_clear_row(&Lcd, 0);
    lcd_send_string_pos(&Lcd, header, 0, 0);
    for (i=0 ; i<3 ; i++){
        lcd_clear_row(&Lcd, (i+1));
        if (Menu.u8Index == i){
            lcd_send_char_pos(&Lcd, '>', (i+1), 0);
        }
        lcd_send_string_pos(&Lcd, items[i], (i+1), 2);
        if (i == 0){
            if (Menu.config.Led1 == true){
                lcd_send_string_pos(&Lcd, "ON", (i+1), 17);
            }
            else{
                lcd_send_string_pos(&Lcd, "OFF", (i+1), 16);
            }
        }
        else if (i == 1){
            if (Menu.config.Led2 == true){
                lcd_send_string_pos(&Lcd, "ON", (i+1), 17);
            }
            else{
                lcd_send_string_pos(&Lcd, "OFF", (i+1), 16);
            }
        }
    }
}

void _menu_screen_info(ButtonPressed){
    uint8_t i;
    char *header = "== Information ==";
    char *items[] = {"MCU",
                     "Counter",
                     "<<- Return"
    };

    // Screen Info only have index 2
    Menu.u8Index = 2;

    switch (ButtonPressed) {
    case MENU_BTN_NONE:
        if (Menu.bRefresh == false){
            return;
        }
        Menu.bRefresh = false;
        break;
    case MENU_BTN_ENTER:
            Menu.eScreen = MENU_SCREEN_MAIN;
            Menu.u8Index = 0;
            Menu.bRefresh = true;
        return;
    case MENU_BTN_SELECT:
        // nothing to do
        return;
    }

    lcd_clear_row(&Lcd, 0);
    lcd_send_string_pos(&Lcd, header, 0, 0);
    for (i=0 ; i<3 ; i++){
        lcd_clear_row(&Lcd, (i+1));
        if (Menu.u8Index == i){
            lcd_send_char_pos(&Lcd, '>', (i+1), 0);
        }
        lcd_send_string_pos(&Lcd, items[i], (i+1), 2);
        if (i == 0){
            lcd_send_string_pos(&Lcd, "FR2433", (i+1), 13);
        }
        else if (i == 1){
            char pcCnt[16];
            sprintf(pcCnt,"%d", Menu.info.Cnt);
            lcd_send_string_pos(&Lcd, pcCnt, (i+1), Lcd.columns - strlen(pcCnt) - 1);
        }
    }
}

void _menu_screen_reset(ButtonPressed){
    uint8_t i;
    char *header = "== Confirm? ==";
    char *items[] = {"YES",
                     "NO"
    };

    switch (ButtonPressed) {
    case MENU_BTN_NONE:
        if (Menu.bRefresh == false){
            return;
        }
        Menu.bRefresh = false;
        break;
    case MENU_BTN_ENTER:
        switch (Menu.u8Index){
        case 0:
            lcd_send_string_pos(&Lcd, "...Resetting...", 1, 2);
            delay_ms(5000);
            WDT_A_start(WDT_A_BASE);
            break;
        case 1:
            Menu.eScreen = MENU_SCREEN_MAIN;
            Menu.u8Index = 0;
            Menu.bRefresh = true;
            return;
        }
        break;
    case MENU_BTN_SELECT:
        Menu.u8Index++;
        if (Menu.u8Index > 1){
            Menu.u8Index = 0;
        }
        break;
    }

    lcd_clear_row(&Lcd, 0);
    lcd_send_string_pos(&Lcd, header, 0, 0);
    for (i=0 ; i<2 ; i++){
        lcd_clear_row(&Lcd, (i+1));
        if (Menu.u8Index == i){
            lcd_send_char_pos(&Lcd, '>', (i+1), 0);
        }
        lcd_send_string_pos(&Lcd, items[i], (i+1), 2);
    }
}

void _menu_process(uint8_t ButtonPressed){


    switch (Menu.eScreen){
    case MENU_SCREEN_MAIN:
        _menu_screen_main(ButtonPressed);
       break;
    case MENU_SCREEN_CONFIG:
        _menu_screen_config(ButtonPressed);
       break;
    case MENU_SCREEN_INFO:
        _menu_screen_info(ButtonPressed);
       break;
    case MENU_SCREEN_RESET:
        _menu_screen_reset(ButtonPressed);
       break;
    } 

}


void _menu_init(){
    Menu.eScreen = MENU_SCREEN_MAIN;
    Menu.u8Index = 0;
    // TODO Show a welcome screen
    lcd_send_string_pos(&Lcd, ">> Menu FR2433 <<", 0, 1);
    lcd_send_string_pos(&Lcd, "Ouroboros", 1, 2);
    lcd_send_string_pos(&Lcd, "Embedded", 2, 2);
    lcd_send_string_pos(&Lcd, "Education", 3, 2);

    Menu.bRefresh = true;
    delay_ms(3000);
}

/* Calbacks */

uint16_t sec1 = 0;
void board_1ms_interrupt(){
    sec1++;
    if (sec1++ > 1000){
        sec1 = 0;
        Menu.info.Cnt++;
        if (Menu.eScreen == MENU_SCREEN_INFO){
            Menu.bRefresh = true;
        }
    }
    
}

/* Main */

void main (void)
{
    uint8_t BtnPressed = MENU_BTN_NONE;
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //Initialize the clock
    board_clock_setup();

    //Initialize gpios 
    board_gpio_setup();
    GPIO_setOutputHighOnPin(USER_LED2_GPIO, USER_LED2_PIN);

    // Initialize timer
    board_timer_setup();

    //Start display LCD
    _lcd_start();

    // Initialize the menu
    _menu_init();

    GPIO_setOutputLowOnPin(USER_LED2_GPIO, USER_LED2_PIN);

    while (true){
        delay_ms(50);
        if (GPIO_getInputPinValue(USER_SELECT_GPIO, USER_SELECT_PIN) == GPIO_INPUT_PIN_LOW){
            BtnPressed = MENU_BTN_SELECT;
        }
        else if (GPIO_getInputPinValue(USER_ENTER_GPIO, USER_ENTER_PIN) == GPIO_INPUT_PIN_LOW){
            BtnPressed = MENU_BTN_ENTER;
        }
        _menu_process(BtnPressed);

        while (GPIO_getInputPinValue(USER_ENTER_GPIO, USER_ENTER_PIN) == GPIO_INPUT_PIN_LOW ||
               GPIO_getInputPinValue(USER_SELECT_GPIO, USER_SELECT_PIN) == GPIO_INPUT_PIN_LOW){
            delay_ms(10);
        }
        BtnPressed = MENU_BTN_NONE;
    }
}
