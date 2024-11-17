#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8880

void handle_client(int client);

int main() {
    int sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(PORT);
    adresse.sin_addr.s_addr = INADDR_ANY;

    if (bind(sk, (struct sockaddr *)&adresse, sizeof(adresse)) < 0) {
        perror("Bind error");
        close(sk);
        return 1;
    }

    if (listen(sk, 2) < 0) {
        perror("Listen error");
        close(sk);
        return 1;
    }

    printf("Server is running on port %d...\n", PORT);

    while (1) {
        int client = accept(sk, NULL, NULL);
        if (client < 0) {
            perror("Accept error");
            continue;
        }

        int processusType = fork();
        if (processusType == 0) {  // Child process
            close(sk);  // Child does not need the main socket
            handle_client(client);
            close(client);
            exit(0);  // Exit child process
        } else {
            close(client);  // Parent does not need the client socket
        }
    }

    close(sk);
    return 0;
}

void handle_client(int client) {
    char message[1024];
    while (1) {
        memset(message, 0, sizeof(message));
        if (recv(client, message, sizeof(message) - 1, 0) <= 0) {
            break;  // Client disconnected
        }

        printf("Received: %s\n", message);  // Debugging received data

        if (strcmp(message, "/exit") == 0) {
            printf("Client disconnected.\n");
            break;
        } else if (strcmp(message, "/download") == 0) {
            // Handle file download
        } else if (strcmp(message, "/upload") == 0) {
            char file_name[50];
            char path[] = "./server_files";
            recv(client, file_name, sizeof(file_name), 0);  // Receive file name
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, file_name);  // Full path with file name
            FILE *fp = fopen(full_path, "w");
            if (!fp) {
                perror("File open error");
                const char *errorMsg = "File could not be created";
                send(client, errorMsg, strlen(errorMsg), 0);
                continue;
            }

            // Receive and write the file content
            while (1) {
                memset(message, 0, sizeof(message));
                int bytes_received = recv(client, message, sizeof(message) - 1, 0);
                if (bytes_received <= 0) {
                    perror("Receive error");
                    break;
                }

                // Debugging output to see what is being received
                printf("Received %d bytes: %s\n", bytes_received, message);

                if (strcmp(message, "EOF_SIGNAL") == 0) {
                    printf("EOF received, stopping file write.\n");
                    break;  // End of file signal
                }

                fwrite(message, sizeof(char), bytes_received, fp);  // Write received data
            }

            fclose(fp);
            printf("File received and saved as %s\n", full_path);
        } else {
            const char *notFound = "Option not found!";
            send(client, notFound, sizeof(notFound), 0);
        }
    }
}
