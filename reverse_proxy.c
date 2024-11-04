#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT_SERVER 8080
#define PORT_PROXY 7000

int main(){
    int server_socket, client_socket, client_socket_2, status;
    struct sockaddr_in server_address, client_address;
    socklen_t addrlen = sizeof(client_address);
    ssize_t size;
    char msg_client[830] = { 0 }; char msg_server[830] = { 0 };

    if(((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) || ((client_socket =socket(AF_INET, SOCK_STREAM, 0)) < 0)){
        perror("Socket falhou!\n");
        return -1;
    }
    // Estabelecendo conexão na porta 8000
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(PORT_PROXY);

    if(bind(client_socket, (struct sockaddr*)&client_address, sizeof(client_address)) < 0){
        perror("bind failed\n");
        return -1;
    }

    if(listen(client_socket, 2) < 0){
        perror("Listen failed\n");
        return -1;
    }

    // Conectando com o server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_SERVER);

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){
        printf("Invalid Address / Address not supported\n");
        return -1;
    }

    if ((status = connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Estou entrando no loop\n");
    if ((client_socket_2 = accept(client_socket, (struct sockaddr*)&client_address, &addrlen)) < 0){
        perror("Error in accept the call");
        return -1;  
    }

    while(1){
        
        printf("Proxy está conectado no client\n");

        size = read(client_socket_2, msg_client, 1024 - 1);
        printf("Leu a mensagem do client: %s\n", msg_client);

        send(server_socket, msg_client, strlen(msg_client) + 1, 0);
        printf("Mandei para o server\n");

        size = read(server_socket, msg_server, 1024 - 1);
        printf("Leu a mensagem que retornou do server: %s\n", msg_server);

        send(client_socket_2, msg_server, strlen(msg_server) + 1, 0);
        printf("Mandou para o client de novo\n");

        memset(msg_client, 0, sizeof(msg_client));
        memset(msg_server, 0, sizeof(msg_server));
    }

    return 0;
}