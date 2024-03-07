#include "msg.h"
  
#define SERVER_IP "192.168.95.13"
  
int main(void) 
{
	int sockfd, addr_len, recv_bytes;
    struct sockaddr_in servaddr;
    char buffer[REMOTE_COMMUNICATION_PKG_LEN];
	struct remote_communication_msg *prcm = (struct remote_communication_msg *)buffer;
	
    // clear servaddr 
    memset(&servaddr, 0, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP); 
    servaddr.sin_port = htons(SERVER_PORT); 
    servaddr.sin_family = AF_INET; 
      
    // create datagram socket 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("Socket create failed with %d\n", errno);
		return RETURN_CODE_FAILED_CREATE_SOCKET;
	}
  
    // request to send datagram
	memset(&buffer, 0, REMOTE_COMMUNICATION_PKG_LEN); 
	prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_QRY;
	prcm->msg_len = strlen("Hello");
	memcpy(&prcm->msg, "Hello", prcm->msg_len);
    if (sendto(sockfd, buffer, sizeof(struct remote_communication_msg) + prcm->msg_len, 0, 
		(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Data send failed with %d\n", errno);
		return RETURN_CODE_FAILED_SEND;
	}
      
	addr_len = sizeof(servaddr);
    if ((recv_bytes = recvfrom(sockfd, buffer, REMOTE_COMMUNICATION_PKG_LEN, 
			0, (struct sockaddr*)&servaddr, &addr_len)) < 0)
	{
		printf("The length of received data is less than 0\n");
	}
	
	printf("%s\n", prcm->msg);
	
	// request to send datagram
	memset(&buffer, 0, REMOTE_COMMUNICATION_PKG_LEN); 
	prcm->msg_type = REMOTE_COMMUNICATION_MSE_TYPE_CFM;
	sprintf(prcm->msg, "How do yo do %s %u", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
	prcm->msg_len = strlen(prcm->msg);
    if (sendto(sockfd, buffer, sizeof(struct remote_communication_msg) + prcm->msg_len, 0, 
		(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Data send failed with %d\n", errno);
		return RETURN_CODE_FAILED_SEND;
	}
	
	addr_len = sizeof(servaddr);
    if ((recv_bytes = recvfrom(sockfd, buffer, REMOTE_COMMUNICATION_PKG_LEN, 
			0, (struct sockaddr*)&servaddr, &addr_len)) < 0)
	{
		printf("The length of received data is less than 0\n");
	}
	
	printf("%s\n", prcm->msg);
  
    // close the descriptor 
    close(sockfd); 
} 
