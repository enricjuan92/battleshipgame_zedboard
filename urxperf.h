/*
 * urxperf.h
 *
 *  Created on: 03/05/2016
 *      Author: francisco.herranz
 */

#ifndef URXPERF_H_
#define URXPERF_H_

#include "system_lib.h"

void    urxperf_recv_callback       (void *arg,
                                     struct udp_pcb *tpcb,
                                     struct pbuf *p,
                                     struct ip_addr *addr,
                                     u16_t port);

int     start_urxperf_application   (msg_attack *result);
void    rx_disconnect               (void);
void    print_urxperf_app_header    (void);

#endif /* URXPERF_H_ */
