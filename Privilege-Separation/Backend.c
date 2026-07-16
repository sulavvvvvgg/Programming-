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

    char password[100];
    char correct_password[] = "hello123";
    char result[100];	
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

    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1)
    {
        printf("Connection failed.\n");
        return 1;
    }

    printf("Frontend connected successfully.\n");

    read(client_socket, password, sizeof(password));

    printf("Received password: %s\n", password);
    
    if (strcmp(password, correct_password) == 0)
    {
       strcpy(result, "Access Granted");
    } 
    else
    {
       strcpy(result, "Access Denied");
    }

    write(client_socket, result, sizeof(result));

    close(client_socket);
    close(server_socket);

    return 0;
}
