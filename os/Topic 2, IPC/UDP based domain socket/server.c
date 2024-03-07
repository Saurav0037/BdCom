#include "msg.h"

void get_content_from_specify_file(int *file_size, char **ptr_content)
{
	char file_name[FILE_NAME_LEN] = {0};
	
	strncpy(file_name, *ptr_content, FILE_NAME_LEN);
	
	{
		int fd;
		
		if ((fd = open(file_name, O_RDONLY)) < 0) 
		{
			sprintf(*ptr_content, "filename %s, %s", file_name, strerror(errno));
			*file_size = strlen(*ptr_content);
		
			return;
		}
		
		{
			struct stat sb;
			if (stat(file_name, &sb) == -1) 
			{
				printf("Get stat of file failed with %s\n", strerror(errno));
				return;
			}
			
			if (sb.st_size >= BUFFER_SIZE)
			{
				*ptr_content = (char *)malloc(sb.st_size + 1);
				if (*ptr_content == NULL)
				{
					printf("Malloc failed when get content of specify file\n");
					return;
				}
			}
			
			read(fd, *ptr_content, sb.st_size);
			*file_size = sb.st_size;
			
			printf("File size: %d bytes\n", *file_size);
		}
		
		close(fd);
	}
	
	
	return;
}

int main(void) 
{
    int socket_fd, client_socket;
	socklen_t addrlen;
	char buffer[BUFFER_SIZE];
    struct sockaddr_un servaddr, cliaddr;

    if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) 
	{
        printf("Creating socket failed with %s\n", strerror(errno));
        return -1;
    }
	
	// bind block
	{
		unlink(DOMAIN_SOCKET_SERVER_PATH);
		
		if (access(DOMAIN_SOCKET_SERVER_PATH, 0) != -1)
		{
			remove(DOMAIN_SOCKET_SERVER_PATH);
		}

		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sun_family = AF_UNIX;
		strcpy(servaddr.sun_path, DOMAIN_SOCKET_SERVER_PATH);

		if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
		{
			printf("Binding socket failed with %s\n", strerror(errno));
			goto release_resource;
		}
	}

    while (1) 
	{
		memset(&cliaddr, 0, sizeof(cliaddr));
		addrlen = sizeof(cliaddr);
		
		// recv
		{
			memset(buffer, 0, BUFFER_SIZE);
		
			if (recvfrom(socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&cliaddr, &addrlen) < 0)
			{
				printf("Receiving socket failed with %s\n", strerror(errno));
				goto release_resource;
			}
		}
		
		{
			int file_size = 0;
			char *ptr_content = buffer;
		
			get_content_from_specify_file(&file_size, &ptr_content);
		}
		
		// send
		{
			int left_need_to_send = file_size, already_send = 0;
			
			while (left_need_to_send)
			{
				if ((already_send = sendto(socket_fd, 
					ptr_content + already_send, 
					left_need_to_send > BUFFER_SIZE?BUFFER_SIZE:left_need_to_send, 0,
					(struct sockaddr*)&cliaddr, addrlen)) < 0)
				{
					printf("Sending data failed with %s\n", strerror(errno));
					goto release_resource;
				}
				
				left_need_to_send -= already_send;
			}
		}
		
		if (file_size >= BUFFER_SIZE)
			free(ptr_content);
    }

release_resource:
    close(socket_fd);

    return 0;
}

