#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main (int argc, char *argv[]) {

  char response[256] = "You've reached me!";

  // Setting up the socket
  int srv_socket;
  srv_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in srv_addresses;
  srv_addresses.sin_family = AF_INET;
  srv_addresses.sin_port = htons(8050);
  srv_addresses.sin_addr.s_addr = INADDR_ANY;


  // Binding socket to the assigned port on the host machine
  bind(srv_socket, (struct sockaddr *) &srv_addresses, sizeof(srv_addresses));

  // Waiting for requests
  listen(srv_socket, 3);

  // Accepting incoming traffic on the same port
  int clt_socket;
  clt_socket = accept(srv_socket, NULL, NULL);

  // Send response string
  send(clt_socket, response, sizeof(response), 0);

  close(srv_socket);

  return 0;
}
