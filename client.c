#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8880

void getMenuOptions();

int main() {
    int sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0) {
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(PORT);  // Use htons for the port
    adresse.sin_addr.s_addr = INADDR_ANY;

    if (connect(sk, (struct sockaddr *)&adresse, sizeof(adresse)) < 0) {
        perror("Connection error");
        close(sk);
        return 1;
    }

    printf("You're now connected to the server!\n");

    char message[1024];
    while (1) {
        getMenuOptions();
        memset(message, 0, sizeof(message));

        printf("Enter the message: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;  // Remove newline character

        send(sk, message, strlen(message), 0);
        printf("Your message was sent successfully\n");

        if (strcmp(message, "/exit") == 0) {
            break;
        } else if (strcmp(message, "/download") == 0) {
            FILE *fp = fopen("./client_files/test", "w");
            if (!fp) {
                perror("File open error");
                continue;
            }

            while (1) {
                memset(message, 0, sizeof(message));
                int bytes_received = recv(sk, message, sizeof(message) - 1, 0);
                if (bytes_received <= 0) {
                    perror("Receive error");
                    break;
                }

                message[bytes_received] = '\0';  // Null-terminate the string

                if (strcmp(message, "EOF_SIGNAL") == 0) {
                    printf("File transfer completed!\n");
                    break;
                } else {
                    fprintf(fp, "%s", message);
                    printf("Writing data to the file...\n");
                }
            }

            fclose(fp);
            printf("File was closed\n");
        } else if (strcmp(message, "/upload") == 0) {
		    char file_name[50];
		    printf("Enter the file name to upload: ");
		    fgets(file_name, sizeof(file_name), stdin);
		    file_name[strcspn(file_name, "\n")] = 0;

		    FILE *fp = fopen(file_name, "r");
		    if (fp == NULL) {
		        perror("File not found");
		    } else {
		        send(sk, file_name, 50, 0);  // Send file name
		        char line[255];
		        while (fgets(line, 255, fp) != NULL) {
		            send(sk, line, strlen(line), 0);  // Send each line
		        }
		        const char *eof_signal = "EOF_SIGNAL";
		        send(sk, eof_signal, strlen(eof_signal), 0);  // Send EOF_SIGNAL to indicate the end
		        printf("File was uploaded successfully!\n");
		        fclose(fp);
		    }
		} else {
            int bytes_received = recv(sk, message, sizeof(message), 0);
            message[bytes_received] = '\0';
            printf("Server response: %s\n", message);
        }
    }

    close(sk);
    return 0;
}

void getMenuOptions() {
    printf("=====================================================\n");
    printf("Select one of these commands:\n");
    printf("/download ===========> To get a file.\n");
    printf("/upload ===========> To send a file.\n");
    printf("/exit ===========> To exit the session.\n");
    printf("=====================================================\n");
}
