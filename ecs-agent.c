#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<stdlib.h>
#include<sys/wait.h>



char* get_name(char* str){
    size_t i = 0, size = 50;
    char* ans = malloc(size);
    memset(ans, 0, size);
    str += 2;
    while(*str != '\0' && i < size){
        ans[i++] = *str;
        str += 1;
    }
    ans[i] = '\0';
    return ans;
}

int ecs_agent(size_t port){
    int socket_desc , client_sock , c , read_size, listening;
    struct sockaddr_in server , client;
    char client_message[2000];
    
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
    server.sin_port = htons( port );
    
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
    listening = 1;
    while(listening) {
        
        memset ( client_message, 0, 2000 );
        if (recv(client_sock , client_message , 2000 , 0) > 0) {
            printf("\n\n\n%s\n\n\nlen=%ld\n", client_message, strlen(client_message));
            if(client_message[0] == 'X') listening = 0;
            send(client_sock , client_message , strlen(client_message), 0);     
        } else {
            puts("recv failed, server will stop listening...");
        }
        //Receive a message from client
        /* if (recv(client_sock , client_message , 2000 , 0) > 0) {
            printf("received message in ecs-agent: %s\n", client_message);
            
            if(client_message[0] == 'X'){
                printf("\n\n\n%s\n\n\nlen=%ld\n", client_message, strlen(client_message));
                listening = 0;
            }else{
                //Send the message back to client
                pid_t pid = fork();
                if (pid < 0) {
                    fprintf(stderr, "fork failed\n");
                    exit(1);
                }else if(pid == 0 && client_message[0] == 'C'){
                    send(client_sock , client_message , strlen(client_message), 0);
                    execlp("docker", "docker", "run", "--name", get_name(client_message), "-id", "ubuntu:latest", "/bin/bash", NULL);
                }
            }
            send(client_sock , client_message , strlen(client_message), 0);
        } else {
            puts("recv failed from ecs-agent");
        } */
    }
    return 1;
}

int main(int argc , char *argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid == 0){
        ecs_agent(9090);
    }else{
        ecs_agent(8080);
    }
}