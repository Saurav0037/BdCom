#ifndef __DOMAIN_SOCKET_MSE_H_
#define __DOMAIN_SOCKET_MSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>

#define DOMAIN_SOCKET_SERVER_PATH "/tmp/ipc_udp_server.sock"
#define DOMAIN_SOCKET_CLIENT_PATH "/tmp/ipc_udp_client.sock"
#define BUFFER_SIZE 1024
#define FILE_NAME_LEN 32

#endif
