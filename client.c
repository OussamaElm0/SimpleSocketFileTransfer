#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void writeOnFile(char *message);

int main()
{
	int sk = socket(AF_INET, SOCK_STREAM, 0);

	if (sk < 0){
		printf("There is an error in the socket\n");
		return 0;
	}

	struct sockaddr_in adresse;

	adresse.sin_family = AF_INET;
	adresse.sin_port = 8880;
	adresse.sin_addr.s_addr = INADDR_ANY;

	int cn = connect(sk, (struct sockaddr *) &adresse, sizeof(adresse));

	if (cn < 0){
		printf("Connot connect, there is an error within the socket!\n");
		return 0;
	}

	printf("You're now connected!\n");
	printf("-------------------------------\n");

	char message[1024];
	while(1) {	
		printf("To get a file enter : /file\n");

		memset(message, 0, sizeof(message));
		
		printf("%d\n", strcmp("d", "d"));
		printf("Enter the message :");
		fgets(message, sizeof(message), stdin);
		message[strcspn(message, "\n")] = 0;  // Remove the newline character

		write(sk, message, strlen(message));
		printf("Your message was sent successfully\n");


		if(strcmp(message, "q") == 0){
			break;
		}else if (strcmp(message, "/file") == 0){
			while(1){
				printf("|||||||||||||||||||\n");
				message[read(sk, message, sizeof(message))] = '\0';  // Null-terminate the string
				
				if(strcmp(message, "d") == 0){
					printf("File transfer completed!\n");
                    break;
				} else {
					writeOnFile(message);
					printf("Writing a new line to the file...\n");
				}
			}
			printf("File was closed\n");
		}
	}

	return 0;
}

void writeOnFile(char * message){
	FILE *fp = fopen("./files/received_file.txt", "a");

	fprintf(fp, "%s", message);
	printf("%s with size : %ld\n", message, sizeof(message));

	fclose(fp);
}