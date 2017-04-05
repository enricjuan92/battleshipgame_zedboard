/*
 * utxperf.h
 *
 *  Created on: 03/05/2016
 *      Author: francisco.herranz
 */

#ifndef UTXPERF_H_
#define UTXPERF_H_

#include "system_lib.h"

int     tx_attack                   (msg_attack msg);
int     start_utxperf_application   (void);
int     get_txstatus                (void);
void    tx_disconnect               (void);
void    print_utxperf_app_header    (void);


#endif /* UTXPERF_H_ */
