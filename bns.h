/*
 * bns.h
 *
 *  Created on: 13/05/2016
 *      Author: laura.vicario
 */

#ifndef BNS_H_
#define BNS_H_

#include "xparameters.h"
#include "xgpio.h"

/* Prototype Functions */
int     read_coordinate_x   (void);
int     read_coordinate_y   (void);
void    read_coordinates    (msg_attack *msg);
int     read_button         (void);


#endif /* BNS_H_ */
