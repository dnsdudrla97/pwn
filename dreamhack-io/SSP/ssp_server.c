// gcc -o ssp_server ssp_server.c -m32 -fstack-protector-all -no-pie
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#define PORT 31337
char *name = "What is your name? ";
char *bye = "See you again.";
char *critical_msg = "THIS_FUNCTION_SHOULD_NOT_BE_CALLED";
void critical(int fd){
    send(fd, critical_msg, strlen(critical_msg), 0);
}
void handler(int fd)
{
    char buf[32] = {};
    send(fd, &fd, 4, 0);
    send(fd, name, strlen(name), 0);
    read(fd, buf, 1024);
    return;
}
int main(void)
{
    int server_fd, new_socket, pid; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                           (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } 
        else if (pid)
        {
            puts("Socket connected");
            close(new_socket);
        } 
        else
        {
            handler(new_socket);
            send(new_socket, bye, strlen(bye), 0);
            return 0;
        }
    }
    return 0; 
}