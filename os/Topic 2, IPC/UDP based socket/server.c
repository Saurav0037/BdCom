#include "msg.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
int main(void) 
{
	int sockfd, addr_len, recv_bytes, is_recv_query;
    struct sockaddr_in servaddr, cliaddr;
	char send_buf[REMOTE_COMMUNICATION_PKG_LEN], recv_buf[REMOTE_COMMUNICATION_PKG_LEN];
	struct remote_communication_msg *send_prcm, *recv_prcm;
	
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(SERVER_PORT); 
    servaddr.sin_family = AF_INET;  
	
	// Create a UDP Socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("Socket create failed with %d\n", errno);
		return RETURN_CODE_FAILED_CREATE_SOCKET;
	}
   
    // bind server address to socket descriptor 
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Socket bind failed with %d\n", errno);
		return RETURN_CODE_FAILED_BIND;
	}
    
    //receive message from server
    addr_len = sizeof(cliaddr);
	send_prcm = (struct remote_communication_msg *)send_buf;
	recv_prcm = (struct remote_communication_msg *)recv_buf;
	
	while(1)
	{
		bzero(&recv_buf, REMOTE_COMMUNICATION_PKG_LEN); 
		if ((recv_bytes = recvfrom(sockfd, recv_buf, REMOTE_COMMUNICATION_PKG_LEN, 
			0, (struct sockaddr*)&cliaddr, &addr_len)) < 0)
		{
			printf("The length of received data is less than 0\n");
			break;
		}
		
		printf("%d %d %s\n", recv_prcm->msg_type, recv_prcm->msg_len, recv_prcm->msg);
		
		switch (recv_prcm->msg_type)
		{
			case REMOTE_COMMUNICATION_MSE_TYPE_QRY:
				is_recv_query = 1;
				send_prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_QRY_ACK;
				sprintf(send_prcm->msg, "Hi %s %u", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
				send_prcm->msg_len = strlen(send_prcm->msg);
				break;
			case REMOTE_COMMUNICATION_MSE_TYPE_CFM:
				if (is_recv_query == 0)
				{
					printf("Didn't recive query from %s %u\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
					continue;
				}
				send_prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_CFM_ACK;
				sprintf(send_prcm->msg, "How do yo do %s %u", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
				send_prcm->msg_len = strlen(send_prcm->msg);
				is_recv_query = 0;
				break;
			default:
				continue;
		}
		
		if (sendto(sockfd, send_prcm, sizeof(struct remote_communication_msg) + send_prcm->msg_len, 0, 
			(struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
		{
			printf("Data send failed with %d\n", errno);
			return RETURN_CODE_FAILED_SEND;
		}
	}
    
	close(sockfd);
} 