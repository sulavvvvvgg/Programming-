#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main()
{
    int server_socket;

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    printf("Socket created successfully.\n");

    close(server_socket);

    return 0;
}
