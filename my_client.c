#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    int mySocket;
    struct sockaddr_in cl;
    char response[256];

    mySocket = socket(AF_INET, SOCK_STREAM, 0);

    if (mySocket < 0) {
        perror("Socket");
        exit(1);
    }

    cl.sin_family = AF_INET;
    cl.sin_port = htons(8050);
    cl.sin_addr.s_addr = INADDR_ANY;

  int status = connect(mySocket, (struct sockaddr *)&cl, sizeof(cl));

  if (status == -1) {
    perror("Connection");
    exit(1);
  }

  recv(mySocket, &response, sizeof(response), 0);

  printf("The server sent: %s\n", response);

  close(mySocket);

  return 0;
}
