#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main(int argc, char *argv[])
{
    /* Create the server */
    int srv_socket;
    srv_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(8000);
    srv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(srv_socket, (struct sockaddr *) &srv_addr, sizeof(srv_addr));

    listen(srv_socket, 5);

    printf("Server is up!");

    int cli_socket;

    char *index = "index.html";

    while(1)
    {
        cli_socket = accept(srv_socket, NULL, NULL);

        char http_response = (char *) &http_response_generator(index);

        send(cli_socket, http_response, sizeof(http_response), 0);
        close(cli_socket);
    }

    return 0;
}

char *http_response_generator(char *filename)
{
    FILE *html_file;
    html_file = fopen(filename, "r");

    char response[1024];
    fgets(response, 1024, html_file);

    char response_header[2048] = "HTTP/1.1 200 OK\r\n\n";
    strcat(response_header, response);

    return response_header;
}
