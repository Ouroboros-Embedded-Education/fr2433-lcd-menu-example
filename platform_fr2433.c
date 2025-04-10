/**
* platform_stm32_c
*
* This file contains the platform code for stm32f4 using HAL library.
* If your Stm32 chipset was from another family, but you're using HAL,
* just modify the include file to your device.
*
*  Created on: Jun, 6 of 2023
*      Author: Pablo Jean
*/

#include "platform/platform.h"

#include "Board.h"

#include <driverlib.h>

void _platform_gpio_write(gpio_t Gpio, uint8_t Val){
	if (Val == 0){
		GPIO_setOutputLowOnPin(Gpio.GPIO, Gpio.pin);
	}
	else{
		GPIO_setOutputHighOnPin(Gpio.GPIO, Gpio.pin);
	}
}

uint8_t _platform_gpio_read(gpio_t Gpio){
	return 0;
}

void _platform_pwm_control(pwm_t PwmHandle, uint8_t duty){
	return;
}

void _platform_delay_ms(uint32_t ms){
	delay_ms(ms);
}

void _platform_delay_us(uint32_t us){
	delay_us(us);
}
