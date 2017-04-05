/*
 * oled.h
 *
 *  Created on: 13/05/2016
 *      Author: laura.vicario
 */

#ifndef OLED_H_
#define OLED_H_

#include "ZedboardOLED.h"
#include <stdio.h>
#include <string.h>

/* Prototype Functions */
int     show_initial            (void);
void    show_failure            (void);
int     show_final              (int end);
void    show_coordinate_x       (int x);
void    show_coordinate_y       (int y);
void    show_result             (char result);

#endif /* OLED_H_ */
