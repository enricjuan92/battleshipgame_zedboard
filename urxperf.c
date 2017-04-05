/*
 * Copyright (c) 2007 Xilinx, Inc.  All rights reserved.
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

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/udp.h"

#include "system_lib.h"

#ifndef __PPC__
#include "xil_printf.h"
#endif

static unsigned rxperf_port = 5001;
static unsigned rxperf_server_running = 0;
static struct udp_pcb udp_general;

void urxperf_recv_callback(void *arg,
                           struct udp_pcb *tpcb,
                           struct pbuf *p,
                           struct ip_addr *addr,
                           u16_t port)
{
    static int first = 1;
    static int expected_id = 0;
    static int n_dropped = 0;
    int recv_id;
    unsigned char pos0, pos1, pos2;
    
    /* first, see if the datagram is received in order -> [FURTHER DEVELOPMENT] */
    recv_id =  *((int *)(p->payload));
    
    if (first)
        expected_id = recv_id;
    else if (expected_id != recv_id)  {
        n_dropped += (recv_id - expected_id);
        expected_id = recv_id;
    }
    
    char message[p->next->len];
    int number;
    
    number=*((int*)p->len);

#if DEBUG
    xil_printf("Checking payload:\nCoordinate X: %02X \r\nCoordinate Y: %02X \r\n",
    		*((unsigned char *)(p->payload)),
    		*((unsigned char *)(p->payload) + 1));
#endif
    
    /* Catching the attack from client to server */
    pos0 = *((unsigned char *)(p->payload));
    pos1 = *((unsigned char *)(p->payload) + 1);
    pos2 = *((unsigned char *)(p->payload) + 2);
    
    if (pos0 <= 7 && pos1 <= 7){
    	attack[0] = pos0;
    	attack[1] = pos1;
    	attack[2] = pos2;
    }

    expected_id++;
    pbuf_free(p);
    
    first = 0;
}

int start_urxperf_application(msg_attack *result)
{
    struct udp_pcb *pcb;
    err_t err;
    unsigned port = rxperf_port;   /* iperf default port */
    static int first = 1;
    
    /* create new UPD PCB structure */
    pcb = udp_new();
    
    if (!pcb)
    {
        xil_printf("Error creating PCB. Out of Memory\r\n");
        return -1;
    }
    
    udp_general = *pcb;
    
    /* bind to iperf @port */
    err = udp_bind(pcb, IP_ADDR_ANY, port);
    if (err != ERR_OK)
    {
#if DEBUG
        xil_printf("Unable to bind to port %d: err = %d\r\n", port, err);
#endif
        return -2;
    }

    udp_recv(pcb, urxperf_recv_callback, NULL);

    /* Retrieve the result of the attack */
    if(*result != NULL)
    {
		(*result)[0] = attack[0]; /* Coordinate X */
		(*result)[1] = attack[1]; /* Coordinate Y */
		(*result)[2] = attack[2]; /*    Symbol    */
    }
    
    rxperf_server_running = 1;
    
    return 0;
}

void rx_disconnect()
{
    udp_disconnect(&udp_general);
    udp_remove(&udp_general);
    
    rxperf_server_running = 0;
}

void print_urxperf_app_header()
{
    xil_printf("%20s %6d %10s %s\r\n", "rxperf server",
               rxperf_port,
               rxperf_server_running ? "RUNNING" : "INACTIVE",
               "$ iperf -c <board ip> -i 5 -t 100 -u -b <bandwidth>");
}

int transfer_urxperf_data(void)
{
    return 0;
}
