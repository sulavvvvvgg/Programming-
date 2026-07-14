#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main()
{
    int client_socket;

    struct sockaddr_un server_address;

    char password[100];

    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (client_socket == -1)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "/tmp/auth_socket");

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("Connection failed.\n");
        return 1;
    }

    printf("Connected to backend successfully.\n");

    printf("Enter password: ");
    scanf("%99s", password);

    write(client_socket, password, sizeof(password));

    printf("Password sent to backend.\n");

    close(client_socket);

    return 0;
}
