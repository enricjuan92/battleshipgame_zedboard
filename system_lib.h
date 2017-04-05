/*
 * system_lib.h
 *
 *  Created on: 04/05/2016
 *      Author: francisco.herranz
 */

#ifndef SYSTEM_LIB_H_
#define SYSTEM_LIB_H_

#include "netif/xadapter.h"

typedef unsigned char mac_addr[6];

/* 
 msg_attack[0]: coordinate X
 msg_attack[1]: coordinate Y
 msg_attack[2]: symbol
 
 Packet format:
 ----------------------------------------
 | Coordinate X | Coordinate Y | Symbol |
 ----------------------------------------
 
 */
typedef unsigned char msg_attack[3];

#define ROWS 8                           /* number of Rows */
#define COLS 8                           /* number of Columns */

#define COORD_X 'x'
#define COORD_Y 'y'

#define ZEDBOARD_A 12                    /* Number ID of Zedboard player A */
#define ZEDBOARD_B 9                     /* Number ID of Zedboard player B */

#define EVRS_AUDIO 1
#define COMMS 0

#define GPIO_CHANNEL_1 1
#define GPIO_CHANNEL_2 2

#define SERVER 0x00
#define CLIENT 0x01

#define BITMASK_INPUT 0xFFFFFFFF
#define BITMAKS_OUTPUT 0x00000000

#define DOWN_BUTTON   2
#define LEFT_BUTTON   4
#define RIGHT_BUTTON  8

#define ERR_OK 0                        /* No error, everything OK. */
#define ERR_PCKT (-1)                   /* Error from packet received, checksum error */

#define DEBUG 0

extern msg_attack attack;

/* dispatch.c */
void        print_headers           (void);
int         start_applications      (int dip_check, msg_attack *result);

/* system_lib.c */
int         assign_role             (void);
void        welcome                 (void);
void        print_attack            (char *location);
void        print_ip_settings       (struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw);
void        print_mac_addr          (mac_addr *macaddr);
void        assign_mac_addr         (int zdboard, mac_addr *macaddr);
void        assign_ip4_addr         (int zdboard, struct ip_addr *ipaddr,
                                                  struct ip_addr *netmask,
                                                  struct ip_addr *gw);

#endif /* SYSTEM_LIB_H_ */
