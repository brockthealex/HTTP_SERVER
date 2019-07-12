//Copyright: Alex Brock 2019


/*Steps for using sockets
1) Create the Socket
2) Identify the Socket
3) Server awaits incoming connections
4) Send/Recieve messages
5) Close the Socket*/

/* -------- 1 -------- 
 A socket (named server_fd) is created with the Socket system call
 int server_fd = socket(domain, type, protocol);
 All the parameters are integers

 Domain (aka: Address Family): Communication domain in which the socket
 should be created. ex: AF_INET (IP), AF_INET6 (IPv6), 
 AF_UNIX (local channel, similar to pipes,) AF_ISO (ISO protocols,)
 and AF_NS (Xeros Network Systems Protocols.)

 Type: type of service, selected according to the properties required by the application: SOCK_STREAM (virtual circuit service,) SOCK_DGRAM (datagram service,) SOCK_RAW (Direct IP Service.) Check with your Domain to see whether a particular service is available

 Protocol: Indicates a specific protocol to use in supporting the sockets operation. Useful in cases where some families have >1 protocol to support a given type of service. 

 For TCP/IP sockets, we want to specify the IP address family (AF_INET) and virtual service (SOCK_STREAM.) Since there's only one form of virtual circuit service. there are no variations of the protocol, so the last argument, protocol, is zero.
*/

/*

*/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
  int server_fd, new_socket;
  long valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  char *hello = "hello from server";

  //creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
    perror("In socket");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  memset(address.sin_zero, '\0', sizeof address.sin_zero);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
    perror("In bind");
    exit(EXIT_FAILURE);
  }
  
  if (listen(server_fd, 100 < 0)) {
    perror("In listen");
    exit(EXIT_FAILURE);
  }
  while(1) {
    printf("\n+++++++++ Waiting for new connection ++++++++++\n\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
      (socklen_t*)&addrlen)) < 0) {
        perror("In accept");
        exit(EXIT_FAILURE);
      }

    char buffer[3000] = {0};
    valread = read( new_socket, buffer, 3000);
    printf("%s\n", buffer);
    write(new_socket, hello, strlen(hello));
    printf("-----------Hello message sent--------------\n");
    close(new_socket);
  }

  return 0;
}
