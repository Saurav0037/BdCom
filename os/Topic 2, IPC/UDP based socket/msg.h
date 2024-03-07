#ifndef __REMOTE_COMMUNICATION_MSE_H_
#define __REMOTE_COMMUNICATION_MSE_H_

#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h> 

#define REMOTE_COMMUNICATION_PKG_LEN 64
#define i8	char
#define i16	short
#define SERVER_PORT 5000 

enum remote_communication_msg_type
{
    REMOTE_COMMUNICATION_MSE_TYPE_QRY = 1,    /* query */
    REMOTE_COMMUNICATION_MSE_TYPE_QRY_ACK,    /* Response query */
    REMOTE_COMMUNICATION_MSE_TYPE_CFM,        /* confirm */
	REMOTE_COMMUNICATION_MSE_TYPE_CFM_ACK	  /* Response confirm */
};

enum return_code
{
	RETURN_CODE_SUCCESS = 0,
	RETURN_CODE_FAILED_CREATE_SOCKET,
	RETURN_CODE_FAILED_SEND,
	RETURN_CODE_FAILED_BIND,
};

#pragma pack(1)
struct remote_communication_msg
{
    i8 msg_type;                             /* message type from remote_communication_msg_type */
    i16 msg_len;                             /* The length of message */
    i8 msg[0];    							 /* content of message */
};
#pragma pack()

#endif
