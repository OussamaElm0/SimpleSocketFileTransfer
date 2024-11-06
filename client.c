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
	adresse.sin_addr.s_addr = INADDR_ANY;

	int cn = connect(sk, (struct sockaddr *) &adresse, sizeof(adresse));

	if (cn < 0){
		printf("Connot connect, there is an error within the socket!\n");
		return 0;
	}

	printf("Connected successfully!\n");

	char message[1024];
	while(1) {	
		memset(message, 0, sizeof(message));
		read(sk, message, sizeof(message));
		message[sizeof(message) - 1] = '\0';

		printf("New message from server : %s\n", message);
		printf("-------------------------------\n");
		
		printf("Enter the message :");
		fgets(message, sizeof(message), stdin);
		message[strcspn(message, "\n")] = 0;  // Remove the newline character

		write(sk, message, strlen(message));


		if(strcmp(message, "q") == 0){
			break;
		}
	}

	return 0;
}