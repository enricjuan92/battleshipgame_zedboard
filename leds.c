/*
 * leds.c
 *
 *  Created on: 13/05/2016
 *      Author: laura.vicario
 */

#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "battleship.h"

void random_sequence(void)
{

	XGpio leds;
	int random;

	XGpio_Initialize(&leds, XPAR_LED_CONTROLLER_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, 1, 0x00000000);

	// Number between 0 and 255
	random = getRandomNumber(0x00, 0xFF);
	xil_printf("Random number = %d\n", random);

	// Write random value in the leds
	Xil_Out32(XPAR_LED_CONTROLLER_0_S00_AXI_BASEADDR, random);

}

void blinking_sequence(void)
{
	XGpio leds;

	XGpio_Initialize(&leds, XPAR_LED_CONTROLLER_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, 1, 0x00000000);

	// Write random value in the leds
	Xil_Out32(XPAR_LED_CONTROLLER_0_S00_AXI_BASEADDR, 0x55);
	sleep(1);
	Xil_Out32(XPAR_LED_CONTROLLER_0_S00_AXI_BASEADDR, 0xAA);
	sleep(1);
}
