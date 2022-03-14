#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc , char *argv[]) {
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    size_t receiving;
    
    //Create socket
    // AF_INET (IPv4 protocol) , AF_INET6 (IPv6 protocol) 
    // SOCK_STREAM: TCP(reliable, connection oriented)
    // SOCK_DGRAM: UDP(unreliable, connectionless)
    // Protocol value for Internet Protocol(IP), which is 0
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 65535 );
    
    //Bind the socket to the address and port number specified
    if( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
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
        return 1;
    }
    puts("Connection accepted");
    
    receiving = 1;
    while(receiving) {
        memset ( client_message, 0, 2000 );
        //Receive a message from client
        if (recv(client_sock , client_message , 2000 , 0) > 0) {
            printf("received message: %s\n", client_message);
            //Send the message back to client
            send(client_sock , client_message , strlen(client_message), 0);
        } else {
            puts("recv failed, server will stop listening...");
            receiving = 0;
        }
    }
    return 0;
}
