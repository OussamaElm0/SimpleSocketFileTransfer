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

    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = 8080;
    adresse.sin_addr.s_addr = INADDR_ANY;

    if (bind(sk, (struct sockaddr *) &adresse, sizeof(adresse)) < 0) {
        perror("Bind error");
        return 1;
    }

    if (listen(sk, 2) < 0) {
        perror("Listen error");
        return 1;
    }

    socklen_t addrlen = sizeof(adresse);
    int client = accept(sk, (struct sockaddr *) &adresse, &addrlen);
    if (client < 0) {
        perror("Accept error");
        return 1;
    }

    char* message_2_send = "Hello! We're glad to meet you.";
    char message_2_recv[1024];
    write(client, message_2_send, strlen(message_2_send));

    read(client, message_2_recv, strlen(message_2_recv));
    printf("New message received : %s\n", message_2_recv);

   	close(client);
    close(sk);

    return 0;
}
