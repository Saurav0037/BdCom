#include "msg.h"

int main(int argc, char *argv[]) 
{
    int socket_fd, client_socket;
	socklen_t addrlen;
	char buffer[BUFFER_SIZE];
    struct sockaddr_un servaddr, cliaddr;
	char file_name[FILE_NAME_LEN] = {0};
	
	if (!(argc == 2 && argv[1]))
	{
		printf("Pls input the filename\n");
		return -1;
	}
	
	strncpy(file_name, argv[1], FILE_NAME_LEN);

    if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) 
	{
        printf("Creating socket failed with %s\n", strerror(errno));
        return -1;
    }
	
	// bind block
	{
		if (access(DOMAIN_SOCKET_CLIENT_PATH, 0) != -1)
		{
			remove(DOMAIN_SOCKET_CLIENT_PATH);
		}
		
		memset(&cliaddr, 0, sizeof(cliaddr));
		cliaddr.sun_family = AF_UNIX;
		strcpy(cliaddr.sun_path, DOMAIN_SOCKET_CLIENT_PATH);
		
		if (bind(socket_fd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0) 
		{
			printf("Binding socket failed with %s\n", strerror(errno));
			goto release_resource;
		}
	}
	
	// for send & recv
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, DOMAIN_SOCKET_SERVER_PATH);
	addrlen = sizeof(servaddr);
	
	{		
		if (sendto(socket_fd, file_name, FILE_NAME_LEN, 0,(struct sockaddr*)&servaddr, addrlen) < 0)
		{
			printf("Sending data failed with %s\n", strerror(errno));
			goto release_resource;
		}
	}
	
	{
		int size = 0, i;
		
		memset(buffer, 0, BUFFER_SIZE);
		while ((size = recvfrom(socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&servaddr, &addrlen)) >= 0)
		{
			for (i = 0; i < size; i++)
				printf("%c", buffer[i]);
			
			if (size < BUFFER_SIZE)
				break;
		}
	}

release_resource:
    close(socket_fd);
	unlink(DOMAIN_SOCKET_CLIENT_PATH);

    return 0;
}

