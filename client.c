#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 7000

int main(int argc, char const* argv[])
{
    
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char str[100];
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status
         = connect(client_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    ssize_t try_send;

    while(1){
        printf("Digite uma mensagem: ");
        fgets(str, sizeof(str), stdin);

        try_send = send(client_fd, str, strlen(str) + 1, 0);

        if (try_send < 0){
            close(client_fd);
            perror("Erro ao enviar mensagem\n");
            continue;
        }

        printf("Mensagem enviada!\n");

        valread = read(client_fd, buffer,
                    1024 - 1); 

        printf("Mensagem recebida do server: %s\n", buffer);

        memset(str, 0, sizeof(str));
        memset(buffer, 0, sizeof(buffer));
    }

    close(client_fd);
    return 0;
}
