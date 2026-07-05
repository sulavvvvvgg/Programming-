#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main()
{
    int server_socket;
    int client_socket;

    struct sockaddr_un server_address;

    // Create a UNIX domain socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Set the socket family and socket path
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "/tmp/auth_socket");

    // Remove the old socket file if it already exists
    unlink("/tmp/auth_socket");

    // Bind the socket to the file path
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("Bind failed.\n");
        return 1;
    }

    // Start listening for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        printf("Listen failed.\n");
        return 1;
    }

    printf("Backend is waiting for a connection...\n");

    // Accept a connection from the frontend
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1)
    {
        printf("Connection failed.\n");
        return 1;
    }

    printf("Frontend connected successfully.\n");

    // Close the client socket
    close(client_socket);

    // Close the server socket
    close(server_socket);

    return 0;
}
