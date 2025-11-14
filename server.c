#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define NUM_CLIENTES 3

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    int client_sockets[NUM_CLIENTES];
    int ids[NUM_CLIENTES];

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    // Permitir reusar el puerto
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Error en setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Configurar dirección
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Escuchar en todas las interfaces
    address.sin_port = htons(PORT);

    // Enlazar socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Error en bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Poner el socket en modo escucha
    if (listen(server_fd, NUM_CLIENTES) == -1) {
        perror("Error en listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    // Aceptar conexiones de NUM_CLIENTES clientes
    for (int i = 0; i < NUM_CLIENTES; i++) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
            perror("Error en accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        client_sockets[i] = new_socket;

        // Recibir ID (un entero)
        int id_net;  // ID en orden de red
        int bytes_recibidos = recv(new_socket, &id_net, sizeof(id_net), 0);
        if (bytes_recibidos <= 0) {
            perror("Error al recibir ID");
            close(new_socket);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        int id_host = ntohl(id_net);  // Convertir a orden de host
        ids[i] = id_host;

        printf("Servidor: recibido ID %d del cliente %d\n", id_host, i + 1);
    }

    // Calcular el líder (máximo ID)
    int lider = ids[0];
    for (int i = 1; i < NUM_CLIENTES; i++) {
        if (ids[i] > lider) {
            lider = ids[i];
        }
    }

    printf("Servidor: el líder elegido es el ID %d\n", lider);

    // Enviar el líder a todos los clientes
    int lider_net = htonl(lider);
    for (int i = 0; i < NUM_CLIENTES; i++) {
        if (send(client_sockets[i], &lider_net, sizeof(lider_net), 0) == -1) {
            perror("Error al enviar líder");
        }
        close(client_sockets[i]);
    }

    close(server_fd);
    printf("Servidor: conexiones cerradas. Fin.\n");
    return 0;
}
