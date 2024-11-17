#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0) {
        perror("Error creating socket");
        return 1;
    }

    printf("Server is running\n");

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(8880);  // Use htons for the port
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

    socklen_t addrlen = sizeof(adresse);
    while (1) {
        int client = accept(sk, (struct sockaddr *)&adresse, &addrlen);
        if (client < 0) {
            perror("Accept error");
            continue;
        }

        int processusType = fork();
        if (processusType == 0) {  // Child process
            close(sk);  // Child does not need the main socket
            printf("-------------------------------\n");
            printf("1 client connected.\n");
            printf("-------------------------------\n");

            char message[1024] = {0};
            while (1) {
                memset(message, 0, sizeof(message));
                if (recv(client, message, sizeof(message) - 1, 0) <= 0) {
                    break;  // Client disconnected
                }

                if (strcmp(message, "/exit") == 0) {
                    printf("-------------------------------\n");
                    printf("1 client disconnected.\n");
                    printf("-------------------------------\n");
                    break;
                } else if (strcmp(message, "/file") == 0) {
                    FILE *fp = fopen("./files_2_send/test1.txt", "r");
                    if (!fp) {
                        perror("File open error");
                        const char *errorMsg = "File not found";
                        send(client, errorMsg, strlen(errorMsg), 0);
                        continue;
                    }

                    char buffer[1024];
                    size_t bytes_read;

                    // Send the file content
                    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                        if (send(client, buffer, bytes_read, 0) < 0) {
                            perror("Send error");
                            break;
                        }
                    }

                    // Send the EOF signal
                    const char *eof_signal = "EOF_SIGNAL";
                    send(client, eof_signal, strlen(eof_signal), 0);
                    fclose(fp);
                    printf("The file was sent successfully.\n");
                } else {
                    const char *notFound = "Option not found!";
                    send(client, notFound, strlen(notFound), 0);
                }
            }

            close(client);
            exit(0);  // Exit child process
        } else {
            close(client);  // Parent does not need the client socket
        }
    }

    close(sk);
    return 0;
}
