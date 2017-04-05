#include "xparameters.h"
#include "xgpio.h"
#include "system_lib.h"
#include "string.h"

// Only needed for SDK versions before 2015
//#include "xutil.h"
//====================================================


int read_coordinate_x(void)
{
	// Pointer and variable declaration
	XGpio switches;
	int sw_check, x;

	// Pointer initialization
	// Check out your own XPAR ID symbol name declared in xparameters.h
	// The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
	XGpio_Initialize(&switches, XPAR_SWITCHES_DEVICE_ID);
	XGpio_SetDataDirection(&switches, 1, 0xffffffff);

    // Read value of switches
    sw_check = XGpio_DiscreteRead(&switches, 1);
    x = (sw_check & 0x3F) / 8;

    return x;
}

int read_coordinate_y(void)
{
	// Pointer and variable declaration
	XGpio switches;
	int sw_check, y;

	// Pointer initialization
	// Check out your own XPAR ID symbol name declared in xparameters.h
	// The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
	XGpio_Initialize(&switches, XPAR_SWITCHES_DEVICE_ID);
	XGpio_SetDataDirection(&switches, 1, 0xffffffff);

    // Read value of switches
    sw_check = XGpio_DiscreteRead(&switches, 1);
    y = sw_check & 0x07;

    return y;
}

void read_coordinates(msg_attack *msg)
{
    // Pointer and variable declaration
    XGpio switches;
    int sw_check, x_coord, y_coord;
    
    /* Pointer initialization */
    /* Check out your own XPAR ID symbol name declared in xparameters.h */
    /* The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID */
    XGpio_Initialize(&switches, XPAR_SWITCHES_DEVICE_ID);
    XGpio_SetDataDirection(&switches, GPIO_CHANNEL_1, BITMASK_INPUT);
    
    // Read value of switches
    sw_check = XGpio_DiscreteRead(&switches, GPIO_CHANNEL_1);
    
    /* X Coordinate */
    x_coord = (sw_check & 0x3F) / 8;
    (*msg)[0] = x_coord;
    
    /* Y Coordinate */
    y_coord = sw_check & 0x07;
    (*msg)[1] = y_coord;
    
}


int read_button(void)
{
	// Pointer and variable declaration
	XGpio buttons;

	// Pointer initialization
	// Check out your own XPAR ID symbol name declared in xparameters.h
	// The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID
	XGpio_Initialize(&buttons, XPAR_BUTTONS_DEVICE_ID);
	XGpio_SetDataDirection(&buttons, 1, 0xffffffff);

	// Read value of buttons
	int psb_check = 0;
	psb_check = XGpio_DiscreteRead(&buttons, 1);
	return psb_check;
}

int switches (void)
{

// Pointer and variable declaration
    XGpio dip, push, leds;
	int i, psb_check, dip_check;

    xil_printf("-- Start of the Program --\r\n");

// Pointer initialization
// Check out your own XPAR ID symbol name declared in xparameters.h
// The format is always XPAR_<NAME_IN_VIVADO>_DEVICE_ID

    XGpio_Initialize(&dip, XPAR_SWITCHES_DEVICE_ID);
	XGpio_SetDataDirection(&dip, 1, 0xffffffff);

	XGpio_Initialize(&push, XPAR_BUTTONS_DEVICE_ID);
	XGpio_SetDataDirection(&push, 1, 0xffffffff);

	XGpio_Initialize(&leds, XPAR_LED_CONTROLLER_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, 1, 0x00000000);

	while (1)
	{
// Read value of buttons
	  psb_check = XGpio_DiscreteRead(&push, 1);

	  xil_printf("Push Buttons Status %x\r\n", psb_check);

// Read value of switches
	  dip_check = XGpio_DiscreteRead(&dip, 1);

	  xil_printf("DIP Switch Status %x\r\n", dip_check);

// Write value of buttons to leds (example)
	  XGpio_DiscreteWrite(&leds, 1, psb_check);

	  // Waiting cycles
	  for (i=0; i<9999999; i++);
	}

}
