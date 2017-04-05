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

#ifndef __TFTPSERVER_H_
#define __TFTPSERVER_H_

#include "lwip/sockets.h"

#define TFTP_MAX_MSG_LEN 600

#define TFTP_DATA_PACKET_MSG_LEN 	512
#define TFTP_DATA_PACKET_HDR_LEN 	4
#define TFTP_DATA_PACKET_LEN 		(TFTP_DATA_PACKET_MSG_LEN + TFTP_DATA_PACKET_HDR_LEN)

#define TFTP_MAX_ACK_LEN 4

#define TFTP_MAX_RETRIES 3
#define TFTP_TIMEOUT_INTERVAL 5

typedef enum {
	TFTP_RRQ = 1,
	TFTP_WRQ = 2,
	TFTP_DATA = 3,
	TFTP_ACK = 4,
	TFTP_ERROR = 5
} tftp_opcode;

typedef enum {
	TFTP_ERR_NOTDEFINED,
	TFTP_ERR_FILE_NOT_FOUND,
	TFTP_ERR_ACCESS_VIOLATION,
	TFTP_ERR_DISKFULL,
	TFTP_ERR_ILLEGALOP,
	TFTP_ERR_UKNOWN_TRANSFER_ID,
	TFTP_ERR_FILE_ALREADY_EXISTS,
	TFTP_ERR_NO_SUCH_USER,
} tftp_errorcode;

typedef struct {
	int op;		/* RRQ/WRQ */
	int fd;

	/* last block read */
	char data[TFTP_MAX_MSG_LEN];	
	int  data_len;

	/* destination ip:port */
	struct ip_addr to_ip;
	int to_port;

	/* next block number */
	int block;

	/* timer interrupt count when last packet was sent */
	/* this should be used to resend packets on timeout */
	unsigned long long last_time;
} tftp_connection_args;

#endif
