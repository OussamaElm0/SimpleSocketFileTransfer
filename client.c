#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int main()
{
	int sk = socket(AF_INET, SOCK_STREAM, 0);

	if (sk < 0){
		printf("There is an error is the socket\n");
		return 0;
	}

	struct sockaddr_in adresse;

	adresse.sin_family = AF_INET;
	adresse.sin_port = 8080;

	int cn = connect(sk, (struct sockaddr *) &adresse, sizeof(adresse));

	if (cn < 0){
		printf("Connot connect, there is an error within the socket!\n");
		return 0;
	}

	printf("Connected successfully!\n");

	char message[1024];

	read(sk, message, sizeof(message));

	printf("%s\n", message);

	printf("Enter the message :");
	fgets(message, 1024, stdin);
	printf("%s", message);
	write(sk, message, strlen(message));

	return 0;
}