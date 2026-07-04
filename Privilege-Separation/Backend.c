#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main()
{
    int server_socket;

    struct sockaddr_un server_address;

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "/tmp/auth_socket");

    unlink("/tmp/auth_socket");

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("Bind failed.\n");
        return 1;
    }

    if (listen(server_socket, 5) == -1)
    {
        printf("Listen failed.\n");
        return 1;
    }

    printf("Backend is waiting for a connection...\n");

    close(server_socket);

    return 0;
}
