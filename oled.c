/*
 * oled.c
 *
 *  Created on: 06/05/2016
 *      Author: laura.vicario
 */

#include "ZedboardOLED.h"
#include <stdio.h>
#include <string.h>
#include "sleep.h"
#include "battleship.h"

/* Show connection message and start the game */
int show_initial(void)
{
	clear();
	print_message("Connection", 0);
	print_message("Established", 1);
	print_message("Starting Game...", 3);
	//wait x seconds clear
	return(1);
}

/* Show failure in the connection */
void show_failure(void)
{
	clear();
	print_message("Connection Fail", 0);
	print_message("Check all", 1);
	print_message("connections and", 2);
	print_message("try again", 3);

}

/* Show end of game, different messages depending on */
int show_final(int end)
{
	clear();
	print_message("End of",0);
	print_message("connection", 1);
	print_message("Game Finished", 3);
	//wait & clear
	sleep(2);
	clear();
	switch (end) {
	case 0x01:
		print_message("The Empire rules",1);
		print_message("the GALAXY",2);
		break;
	case 0x02:
		print_message("You saved the",1);
		print_message("   GALAXY!!!!",2);
		break;
	default:
		clear();
		print_message("FAKE",1);
		break;
	}
	return(1);

}

void show_coordinate_x(int x)
{
	clear();
	switch (x) {
	case 0x00:
		print_message("x = 0",1);
		break;
	case 0x01:
		print_message("x = 1",1);
		break;
	case 0x02:
		print_message("x = 2",1);
		break;
	case 0x03:
		print_message("x = 3",1);
		break;
	case 0x04:
		print_message("x = 4",1);
		break;
	case 0x05:
		print_message("x = 5",1);
		break;
	case 0x06:
		print_message("x = 6",1);
		break;
	case 0x07:
		print_message("x = 7",1);
		break;
	default:
		clear();
		break;
	}
}

void show_coordinate_y(int y)
{
	switch (y) {
	case 0x00:
		print_message("y = 0",2);
		break;
	case 0x01:
		print_message("y = 1",2);
		break;
	case 0x02:
		print_message("y = 2",2);
		break;
	case 0x03:
		print_message("y = 3",2);
		break;
	case 0x04:
		print_message("y = 4",2);
		break;
	case 0x05:
		print_message("y = 5",2);
		break;
	case 0x06:
		print_message("y = 6",2);
		break;
	case 0x07:
		print_message("y = 7",2);
		break;
	default:
		clear();
		break;
	}
}

void show_result(char result)
{
	switch (result) {
	case HIT:
		clear();
		print_message("HIT", 2);
		break;
	case MISS:
		clear();
		print_message("MISS", 2);
		break;
	case WATER:
		clear();
		print_message("WATER", 2);
		break;
	default:
		clear();
		break;
	}
}
