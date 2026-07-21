#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int server_socket;
    int client_socket;

    struct sockaddr_un server_address;

    char password[100];
    char correct_password[] = "hello123";
    char result[100];

    uid_t user_id;
    uid_t new_user = 1000;

    // Create socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Set socket address
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "/tmp/auth_socket");

    // Remove old socket file
    unlink("/tmp/auth_socket");

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("Bind failed.\n");
        return 1;
    }

    chmod("/tmp/auth_socket", 0666);


    // Listen for connections
    if (listen(server_socket, 5) == -1)
    {
        printf("Listen failed.\n");
        return 1;
    }

    printf("Backend is waiting for a connection...\n");

    // Accept connection
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == -1)
    {
        printf("Connection failed.\n");
        return 1;
    }

    printf("Frontend connected successfully.\n");

    // Receive password
    if (read(client_socket, password, sizeof(password)) <= 0)
    {
        printf("Failed to receive password.\n");

        close(client_socket);
        close(server_socket);

         	return 1;
    }
    printf("Received password: %s\n", password);

    // Show current effective user ID
    user_id = geteuid();
    printf("Effective User ID: %d\n", user_id);

    // Drop privileges if running as root
    if (user_id == 0)
    {
        if (setresuid(new_user, new_user, new_user) == 0)
        {
            printf("Privileges dropped successfully.\n");
        }
        else
        {
            printf("Failed to drop privileges.\n");
        }

        printf("New Effective User ID: %d\n", geteuid());
    }

    // Check password
    if (strcmp(password, correct_password) == 0)
    {
        strcpy(result, "Access Granted");
    }
    else
    {
        strcpy(result, "Access Denied");
    }

    // Send result back to frontend
    if (write(client_socket, result, sizeof(result)) == -1)
    {
        printf("Failed to send result.\n");
    }

    memset(password, 0, sizeof(password));

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
