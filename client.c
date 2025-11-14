#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ID_DEL_CLIENTE>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int id = atoi(argv[1]);

    int sock = 0;
    struct sockaddr_in serv_addr;

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir IP en texto a binario
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Dirección inválida/ no soportada");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Conectarse al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error en connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Cliente (ID=%d): conectado al servidor.\n", id);

    // Enviar ID
    int id_net = htonl(id);
    if (send(sock, &id_net, sizeof(id_net), 0) == -1) {
        perror("Error al enviar ID");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Recibir líder
    int lider_net;
    int bytes_recibidos = recv(sock, &lider_net, sizeof(lider_net), 0);
    if (bytes_recibidos <= 0) {
        perror("Error al recibir líder");
        close(sock);
        exit(EXIT_FAILURE);
    }

    int lider = ntohl(lider_net);
    printf("Cliente (ID=%d): el líder elegido es: %d\n", id, lider);

    close(sock);
    return 0;
}
