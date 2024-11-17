#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void writeOnFile(const char *message);
void getMenuOptions();

int main() {
    int sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0) {
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(8880);  // Use htons for the port
    adresse.sin_addr.s_addr = INADDR_ANY;

    if (connect(sk, (struct sockaddr *)&adresse, sizeof(adresse)) < 0) {
        perror("Connection error");
        close(sk);
        return 1;
    }

    printf("You're now connected!\n");
    printf("-------------------------------\n");

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
        } else if (strcmp(message, "/file") == 0) {
            FILE *fp = fopen("./received_files/file1.txt", "w");
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
        } else {
            recv(sk, message, sizeof(message) - 1, 0);
            message[recv(sk, message, sizeof(message), 0)] = '\0';
            printf("========> %s <========\n", message);
        }
    }

    close(sk);
    return 0;
}

void getMenuOptions() {
    printf("=====================================================\n");
    printf("Select one of these commands:\n");
    printf("/file ===========> To get a file.\n");
    printf("/exit ===========> To exit the session.\n");
    printf("=====================================================\n");
}
