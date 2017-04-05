/*
 * Copyright (c) 2007 Xilinx, Inc. All rights reserved.
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
#include "xparameters.h"
#include "platform.h"
#include "platform_config.h"
#include "lwipopts.h"
#include "xgpio.h"

#include "system_lib.h"
#include "utxperf.h"
#include "urxperf.h"
#include "ZedboardOLED.h"
#include "adventures_with_ip.h"
#include "xuartps.h"
#include "sleep.h"
#include "xtime_l.h"
#include "xparameters.h"
#include "bns.h"
#include "leds.h"
#include "oled.h"
#include "battleship.h"

#ifndef __PPC__
#include "xil_printf.h"
#endif

int read_button(void);
void platform_enable_interrupts();
void lwip_init(void);

#if LWIP_DHCP==1
extern volatile int dhcp_timoutcntr;
err_t dhcp_start(struct netif *netif);
#endif
extern volatile int TxPerfConnMonCntr;

unsigned int first_attack = 1;

msg_attack attack = {0xFF, 0xFF, 0xFF};
msg_attack mycoordinate = {0xFF, 0xFF, 0xFF};
msg_attack enemycoordinate = {0xFF, 0xFF, 0xFF};

int main() {
    
    struct netif *netif, server_netif;
    struct ip_addr ipaddr, netmask, gw;
    mac_addr mac_ethernet_address;
    
    /* the mac address of the board. this should be unique per board */
    assign_mac_addr(ZEDBOARD_A, &mac_ethernet_address);
    print_mac_addr(&mac_ethernet_address);
    
    netif = &server_netif;
    
    if (init_platform() < 0)
    {
        xil_printf("ERROR initializing platform.\r\n");
        return -1;
    }
    
    xil_printf("\r\n\r\n");
    xil_printf("-----lwIP RAW Mode Demo Application ------\r\n");
    
    /* initliaze IP addresses to be used */
#if (LWIP_DHCP==0)
    assign_ip4_addr(ZEDBOARD_A, &ipaddr, &netmask, &gw);
    print_ip_settings(&ipaddr, &netmask, &gw);
#endif
    lwip_init();
    
    /* Add network interface to the netif_list, and set it as default */
    if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR))
    {
        xil_printf("Error adding N/W interface\r\n");
        return -1;
    }
    netif_set_default(netif);
    
    /* specify that the network if is up */
    netif_set_up(netif);
    
    /* now enable interrupts */
    platform_enable_interrupts();
    
    XGpio button, leds;
    
    XGpio_Initialize(&button, XPAR_BUTTONS_DEVICE_ID);
    XGpio_SetDataDirection(&button, GPIO_CHANNEL_1, BITMASK_INPUT);
    
    XGpio_Initialize(&leds, XPAR_LED_CONTROLLER_0_DEVICE_ID);
    XGpio_SetDataDirection(&leds, GPIO_CHANNEL_1, BITMASK_INPUT);

    print_headers();
    
    while (1) {

    	clear();
        
        int i, res;
        int state;
        
        /* Game and Comms definitions */
        gameboard myBoard;
        gameboard enemyBoard;
        
        /* Defined the number and type of ships */
        fleet ships = {{ 'c', 5, "Carrier"    , 0, FALSE},
                       { 'b', 4, "Battleship" , 0, FALSE},
                       { 'r', 3, "Cruiser"    , 0, FALSE},
                       { 's', 3, "Submarine"  , 0, FALSE},
                       { 'd', 2, "Destroyer"  , 0, FALSE}};


        /* Initialize boards */
        generate_board(ships, &myBoard, &enemyBoard);
        
        /* Show init sequence for configuration */
        welcome();
        
        xil_printf("\n> Your board:\n");
        show_myBoard(myBoard);
        sleep(1);

        /* Establish connection SERVER mode */
        if (start_applications(SERVER, NULL) != -1){
            show_initial();
        
        }else{
            show_failure();
            continue;
        }
        
        /* Welcoming sequence */
        for (i = 0; i < 3; i++)
            blinking_sequence();
        
        Xil_Out32(XPAR_LED_CONTROLLER_0_S00_AXI_BASEADDR, 0x00);
        
        /* Check with buttons the proper role: Server or Client */
        state = assign_role();
        
        while (TRUE){
            
            switch (state)
            {
                /* Client flow: it handles the transmission */
                case CLIENT:
                    
                    while(TRUE){
                        
                        if (!get_txstatus())
                        {
                            rx_disconnect();
                            sleep(1);
                            
                            start_applications(CLIENT, NULL);
                            print_headers();
                        }
                        
                        xil_printf("> YOUR TURN: Enemy Board:\n");
                        showBoard(enemyBoard, FALSE);
                        
                        xil_printf("> YOUR TURN, USE THE SWITCHES TO ATTACK!\n");
                        xil_printf("> Please, push Down Button to read coordinates...\n\n");
                        
                        while(read_button() != DOWN_BUTTON);

                        attack[0] = read_coordinate_x();
                        attack[1] = read_coordinate_y();
                        sleep(1);
                        
                        mycoordinate[0] = attack[0];
                        mycoordinate[1] = attack[1];
                        
                        /* Once the coordinates are read, the attack is sent */
                        res = tx_attack(attack);

                        xil_printf("> Coordinate X to attack: %d\n", attack[0]);
                        xil_printf("> Coordinate Y to attack: %d\n", attack[1]);
                        xil_printf("> Attack shown in the OLED screen too.\n");

                        print_attack("> CLIENT: TX-ATTACK");

                        if(res != ERR_OK)
                        {
                            xil_printf("> There are some transmission problems.\n Please, try again. \n");
                            xil_printf("> Trying again the transmission.\n");
                            continue;
                        }
                        
                        /* Show the coordinate board that player want to attack */
                        show_coordinate_x(attack[0]);
                        show_coordinate_y(attack[1]);
                        
                        /* Reinitialize attack variable */
                        memset(attack, 0xFF, sizeof(attack));
                        first_attack = FALSE;
                        
                        /* Transition from CLIENT to SERVER */
                        xil_printf("> To finalize your turn: ");
                        xil_printf("If you have performed the attack succesfully, please, press the right button.\n");
                        while (read_button() != RIGHT_BUTTON);

                        /* If transmission was OK change role */
                        if(!res)
                        {
                        	tx_disconnect();
                            state = SERVER;
                        	
                            break;
                        }
                    }
                    
                    break;
                    
                /* Server flow: it handles the reception */
                case SERVER:
                    
                	xil_printf("\n> Waiting the attack of the other Player\n");

                	sleep(1);
                    start_applications(SERVER, &attack);
                    print_headers();
                    
                    /* Infinite loop to ensure receive a packet */
                    while(attack[0] == 0xFF &&
                          attack[1] == 0xFF &&
                          attack[2] == 0xFF)
                    {
                        /* Start as SERVER and receive the attack sent by the client */
                        xemacif_input(netif);
                    }
                    
                    print_attack("> SERVER: after infinite loop -> packets received\n");

                    if (!first_attack)
                    {
                    	/* Update symbol from my attack on the enemy board */
                        mycoordinate[2] = attack[2];
                        
                        /* Update the enemy's board with the result of your attack */
                        updateBoard(&enemyBoard, mycoordinate);
                        
                        xil_printf("> WAITING FOR YOUR TURN: Enemy Board:\n");
                        showBoard(enemyBoard, FALSE);
                        
                        /* Result of your attack onto the enemy board */
                        show_result(attack[2]);
                        
                        memset(mycoordinate, 0xFF, sizeof(mycoordinate));
                    }

                    /* Check the attack of the enemy on MyBoard */
                    xil_printf("> SERVER: check attack and update My Board\n");
                    check_attack(&myBoard, &ships, &attack);
                    
                    /* Get the coordinates that the enemy has hit so as to update my board properly */
                    enemycoordinate[0] = attack[0];
                    enemycoordinate[1] = attack[1];
                    enemycoordinate[2] = 0xFF;

                    /* Update MyBoard with the result of the attack from the result of the attack */
                    updateBoard(&myBoard, enemycoordinate);
                    xil_printf("> The other Player has attack you.\nResult of the attack shown in the OLED.\n\n");
                    xil_printf("> My Board:\n");
                    showBoard(myBoard, FALSE);
                    
                    /* Show result of the enemy's attack on the OLED */
                    show_result(attack[2]);
                    
                    /* Check if game is finished */
                    if(is_winner(ships))
                    {
                        show_final(1);
                        goto end_game;
                    }

                    /* Require the change of role */
                    xil_printf("> Waiting until the other player decides to end his/her turn pressing the right button...\n");
                    xil_printf("> Please press the left button.\n");
                    
                    while (read_button() != LEFT_BUTTON);

                    first_attack=0;
                    rx_disconnect();
                    state = CLIENT;
                    
                    break;
            }
        }
        return 0;
        
    }
    
end_game:
    tx_disconnect();
    rx_disconnect();
    sleep(2);
    
    clear();
    print_message("GAME OVER!", 2);

    /* never reached */
    cleanup_platform();
    
    while(1);
    return 0;
}
