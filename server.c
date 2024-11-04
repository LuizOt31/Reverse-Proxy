#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#define PORT 8080

void toUpper(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toupper(str[i]); // Converte cada caractere para maiúsculo
        i++;
    }
}

int main(){
    int server_socket, socket_proxy;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[830] = { 0 };
    ssize_t size;

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket falhou!\n");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind falhou\n");
        return -1;
    }

    if(listen(server_socket, 2) < 0){
        perror("Listen Falhou\n");
        return -1;
    }

    printf("Entrando no loop\n");
    if ((socket_proxy = accept(server_socket, (struct sockaddr*)&address, &addrlen)) < 0){
        perror("Erro em aceitar a chamada\n");
        return -1;
    }

    while(1){
        
        printf("Server está conectado no Proxy\n");
        size = read(socket_proxy, buffer, 830 - 1);
        printf("Mensagem recebida: %s\n Tamanho da mensagem: %ld\n", buffer, strlen(buffer));

        if (buffer[strcspn(buffer, "\n")] == '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove o '\n'
        }

        if (strcmp(buffer, "exit") == 0) {
            close(server_socket);
            close(socket_proxy);
            exit(EXIT_SUCCESS);
        }

        // Verifica se a requisição é para "exit"
        if (strstr(buffer, "exit") != NULL) {
            close(socket_proxy);
            break; // Sair do loop e fechar o servidor
        }



        toUpper(buffer);

	    char response[930];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: close\r\n"
                 "\r\n"
                 "%s",
                 strlen(buffer), buffer);

        ssize_t sent_size = send(socket_proxy, buffer, strlen(buffer), 0);
        if (sent_size < 0) {
            perror("Erro no send\n");
            close(socket_proxy);
            continue;
        }

        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
