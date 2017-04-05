/*
 * system_lib.c
 *
 *  Created on: 04/05/2016
 *      Author: francisco.herranz
 */

#include "system_lib.h"
#include "bns.h"

#ifndef __PPC__
#include "xil_printf.h"
#endif

int assign_role(void)
{
    unsigned int role;
    
    xil_printf("Please press Right Button to configure as Server and Left Button to start as Client.\n");

    while (1) {
        
        /* Infinite loop to lock until it's detected that the proper button is pushed */
        role = read_button();
        
        if (role == LEFT_BUTTON){
            return CLIENT;
            
        }else if (role == RIGHT_BUTTON){
            return SERVER;
            
        }else{
            continue;
        }
    }
    
    return 0;
}

void print_attack(char *location)
{

#if DEBUG
	xil_printf("Location: %s\n", location);
	xil_printf("attack[0] = %02X \n", attack[0]);
	xil_printf("attack[1] = %02X \n", attack[1]);
	xil_printf("attack[2] = %02X \n", attack[2]);
#endif
}

void welcome(void)
{
    // Show instructions
    xil_printf("Welcome to Empire vs. Rebels \n");
    xil_printf("Rules of the game \n");
    xil_printf("1. There are five types of ship in the map\n");
    xil_printf("[c] Carrier has 5 cells, [b] Battleship has 4 cells, \n");
    xil_printf("[r] Cruiser has 3 cells, [s] Submarine has 3 cells and \n");
    xil_printf("[d] Destroyer has 2 cells. \n");
    xil_printf("2. The computer will randomly create your map. \n");
    xil_printf("3. The coordinates of the attacks are controlled \n");
    xil_printf("by the switches. Switches from 5-3 set the x coordinate, \n");
    xil_printf("while switches from 2-0 set the y coordinate. \n");
    xil_printf("Once you have selected the coordinates \n");
    xil_printf("press the down button so as to send them to your opponent. \n");
    xil_printf("4. Please when you finish reading press [q] key to start the game. \n");
    
    //Stream audio
    init_audio();
}

static void print_ip(char *msg, struct ip_addr *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\r\n",
			ip4_addr1(ip),
			ip4_addr2(ip),
			ip4_addr3(ip),
			ip4_addr4(ip));
}

void print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{
	print_ip("Board IP:       ", ip);
	print_ip("Netmask :       ", mask);
	print_ip("Gateway :       ", gw);
}

void print_mac_addr(mac_addr *macaddr)
{
	xil_printf("Assigned MAC ETHERNET ADDRESS: %02X : %02X : %02X : %02X : %02X : %02X \r\n",
			(*macaddr)[0],
			(*macaddr)[1],
			(*macaddr)[2],
			(*macaddr)[3],
			(*macaddr)[4],
			(*macaddr)[5]);
}

void assign_ip4_addr(int zdboard, struct ip_addr *ipaddr,
		struct ip_addr *netmask, struct ip_addr *gw)
{
	IP4_ADDR(ipaddr, 147, 83, 81, (220 + zdboard));
	IP4_ADDR(netmask, 255, 255, 255, 0);
	IP4_ADDR(gw, 147, 83, 81, 1);
}

void assign_mac_addr(int zdboard, mac_addr *macaddr) {
	unsigned char pos4, pos5, pos6;

	switch (zdboard) {
	case 1:
		pos4 = 0x28;
		pos5 = 0xb8;
		pos6 = 0xd5;
		break;
	case 2:
		pos4 = 0x7f;
		pos5 = 0x9f;
		pos6 = 0x39;
		break;
	case 3:
		pos4 = 0x7d;
		pos5 = 0x7c;
		pos6 = 0xde;
		break;
	case 4:
		pos4 = 0xd7;
		pos5 = 0x9f;
		pos6 = 0x67;
		break;
	case 5:
		pos4 = 0xa4;
		pos5 = 0xd7;
		pos6 = 0x4b;
		break;
	case 6:
		pos4 = 0x1c;
		pos5 = 0xba;
		pos6 = 0x7f;
		break;
	case 7:
		pos4 = 0xa4;
		pos5 = 0xd2;
		pos6 = 0x13;
		break;
	case 8:
		pos4 = 0x7d;
		pos5 = 0x25;
		pos6 = 0x8d;
		break;
	case 9:
		pos4 = 0x8a;
		pos5 = 0x79;
		pos6 = 0x38;
		break;
	case 10:
		pos4 = 0x0f;
		pos5 = 0xea;
		pos6 = 0x85;
		break;
	case 11:
		pos4 = 0x54;
		pos5 = 0x66;
		pos6 = 0xcb;
		break;
	case 12:
		pos4 = 0xfa;
		pos5 = 0xf6;
		pos6 = 0xab;
		break;
	default:
		pos4 = 0x00;
		pos5 = 0x00;
		pos6 = 0x00;
		break;
	}

	(*macaddr)[0] = 0x00;
	(*macaddr)[1] = 0x16;
	(*macaddr)[2] = 0x3e;

	(*macaddr)[3] = pos4;
	(*macaddr)[4] = pos5;
	(*macaddr)[5] = pos6;
}
