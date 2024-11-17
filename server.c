#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int getAvailableFiles();

int main() {
    int sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0) {
        perror("Error creating socket");
        return 1;
    }

    printf("Server is running\n");

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = 8880;
    adresse.sin_addr.s_addr = INADDR_ANY;

    if (bind(sk, (struct sockaddr *) &adresse, sizeof(adresse)) < 0) {
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
    while(1){
        int client = accept(sk, (struct sockaddr *) &adresse, &addrlen);
        int processusType = fork(); // Parent or Children

        if(processusType == 0){
            if (client < 0) {
                perror("Accept error");
                close(sk);
                return 1;
            }
            
            printf("-------------------------------\n");
            printf("1 client connected.\n");
            printf("-------------------------------\n");

            char message[1024] = {0};
            while (1) {
                memset(message, 0, sizeof(message));
                recv(client, message, sizeof(message) - 1, 0);

                if (strcmp(message, "/exit") == 0){
                    printf("-------------------------------\n");
                    printf("1 client disconected\n");
                    printf("-------------------------------\n");
                    break;
                }else if (strcmp(message, "/file") == 0){
                    FILE *fp = fopen("./files_2_send/test1.txt", "r");

                    char line[255];

                    printf("The sending start;\n");
                    while (fgets(line, 255, fp) != NULL){
                        send(client, line, strlen(line), 0);
                        printf("%s", line);
                    }
                    printf("-------------------------------\n");

                    const char *eofSignal = "d";
                    send(client, eofSignal, strlen(eofSignal),0);
                    fclose(fp);
                    printf("The file was sent successfully.\n");
                } else {
                    strcpy(message, "Option not found!");
                    send(client, message, strlen(message), 0);
                }


                printf("-------------------------------\n");

                
            }

            close(client);    
        } else {
            continue;
        }
    }

    close(sk);
    return 0;
}