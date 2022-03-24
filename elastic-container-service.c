#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <time.h>

char* get_name(char* str){
    size_t i = 0, size = 50;
    char* ans = malloc(size);
    str += 2;
    while(*str != '\0' && i < size){
        ans[i++] = *str;
        str += 1;
    }
    ans[i] = '\0';
    return ans;
}

void socket_client(char* command, char *message, size_t port){
    //printf("IN SOCKET_CLIENT: %s %s\n\n", command, message);
    int sock;
	struct sockaddr_in server;
	char server_reply[2000];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("connect failed. Error");
		return;
	}
	
	puts("Connected\n");
	
	//keep communicating with server
    size_t sending = 1;
	while(sending) {

        char formated_message[51];

        strcpy( formated_message, command );
        strcat( formated_message, message );
		
        //printf("FORMATED MESSAGE = %s\n", formated_message);
		//Send some data
		if( send(sock , formated_message , strlen(formated_message) , 0) < 0) {
			puts("Send failed");
			return;
		} else {
            puts("send ok");
            sending = 0;
        }
		
		//Receive a reply from the server
        memset ( server_reply, 0, 2000 );
		if( recv(sock , server_reply , 2000 , 0) < 0) {
			puts("recv failed");
		} else {
            puts("recv ok");
        }
		
		puts("Server reply :");
		puts(server_reply);
	}
	
	close(sock);
}

void subscribe_host(){
    // SUBSCRIBE_HOST
    // child process because return value zero
    // socket client
    int sock;
    char message[1000] , server_reply[2000];
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    size_t receiving, sending;
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("connect failed. Error from subscribe_host()");
        return;
    }
    
    puts("Connected\n");
    
    //keep communicating with server
    sending = 1;
    /* while(sending) {

        // DATA TO SEND
        printf("Enter message : ");
        scanf("%s" , message);
        
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0) {
            puts("Send failed");
            return 1;
        } else {
            puts("send ok");
        }
        
        //Receive a reply from the server
        memset ( server_reply, 0, 2000 );
        if( recv(sock , server_reply , 2000 , 0) < 0) {
            puts("recv failed");
            sending = 0;
        } else {
            puts("recv ok");
        }
        
        puts("Server reply :");
        puts(server_reply);
    } */
    
    close(sock);
}

void admin_container(){
    // parent process because return value non-zero.
    // socket server
    int socket_desc , client_sock, c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    size_t receiving, sending;
    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created from parent");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 7070 );
    
    //Bind the socket to the address and port number specified
    if( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return;
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
        perror("accept failed");
        return;
    }
    puts("Connection accepted");
    
    receiving = 1;
    while(receiving) {
        memset ( client_message, 0, 2000 );
        //Receive a message from client
        if (recv(client_sock , client_message , 2000 , 0) > 0) {

            char *name = get_name(client_message);
            char command = client_message[0];

            if (command == 'C' || command == 'S' || command == 'R' || command == 'D' || command == 'X'){

                int r = rand()%2;
                int port;

                if(r){
                    port = 8080;
                }else{
                    port = 9090;
                }

                if(command == 'C'){
                    FILE * fp;
                    fp = fopen ("containers.txt", "a");
                    fprintf(fp, "%s:%d\t%s\n", "127.0.0.1", port, name);
                    fclose(fp);
                }
                
                socket_client(&command, name, port);

            }else{
                puts("Invalid command received @ admin_container.");
            }
        
            // Bloque de ejecucion del padre
            printf("received message: %s\n", client_message);
            //Send the message back to client
            send(client_sock , client_message , strlen(client_message), 0);
            wait(NULL); // puede fallar?
            
        } else {
            puts("recv failed, server will stop listening...");
            receiving = 0;
        }
    }
}

int main(int argc , char *argv[]) {

    srand(time(NULL));

    int id = fork();

    if (id == 0) {
        subscribe_host();
    }
    else {
        admin_container();
    }

    
    return 0;
}
