#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int clientsCount = 0;

    int sk = socket(AF_INET, SOCK_STREAM, 0);
    if (sk < 0) {
        perror("Error creating socket");
        return 1;
    }

    printf("Server is running\n");

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = 8080;
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

    while (clientsCount < 2) {
        socklen_t addrlen = sizeof(adresse);
        int client = accept(sk, (struct sockaddr *) &adresse, &addrlen);
        if (client < 0) {
            perror("Accept error");
            close(sk);
            return 1;
        }
        
        clientsCount++;
        printf("Client %d connected.\n", clientsCount);

        char* message_2_send = "Hello! We're glad to meet you.";
        write(client, message_2_send, strlen(message_2_send));

        char message[1024] = {0};
    
        read(client, message, sizeof(message) - 1);
        printf("New message received: %s\n", message);

        close(client);
        clientsCount--;
    }

    close(sk);
    return 0;
}
