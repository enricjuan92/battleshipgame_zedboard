/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "lwip/inet.h"
#include "lwip/ip_addr.h"
#include "xgpio.h"
#ifndef __PPC__
#include "xil_printf.h"
#endif
#include "config_apps.h"
#ifndef __arm__
#include "xil_exception.h"
#endif

#include "system_lib.h"
#include "oled.h"
#include "urxperf.h"
#include "utxperf.h"

void transfer_utxperf_data();
void transfer_urxperf_data();

void print_headers()
{
#if DEBUG
    xil_printf("\r\n");
    xil_printf("%20s %6s %s\r\n", "Server", "Port", "Connect With..");
    xil_printf("%20s %6s %s\r\n", "--------------------", "------", "--------------------");

    if (INCLUDE_TXUPERF_CLIENT)
    	print_utxperf_app_header();

    if (INCLUDE_RXUPERF_CLIENT)
    	print_urxperf_app_header();

    xil_printf("\r\n");
#endif
}

int start_applications(int dip_check, msg_attack *result)
{
    if (INCLUDE_TXUPERF_CLIENT && dip_check == CLIENT)
    {
    	return start_utxperf_application();
    }

    if (INCLUDE_RXUPERF_CLIENT && dip_check == SERVER)
    {
    	return start_urxperf_application(result);
    }

    return 0;
}

void transfer_data()
{
	if (INCLUDE_TXUPERF_CLIENT)
		transfer_utxperf_data();

	if (INCLUDE_RXUPERF_CLIENT)
		transfer_urxperf_data();
}
