#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // Para getaddrinfo y estructuras modernas

#pragma comment(lib, "ws2_32.lib") // Librería de Winsock

#define PORT 9000
#define SERVER_ADDRESS "127.0.0.1"

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock\n";
        return -1;
    }

    // Crear el socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket\n";
        WSACleanup();
        return -1;
    }

    // Resolver la dirección del servidor usando getaddrinfo
    struct addrinfo hints = { 0 }, * result = nullptr;
    hints.ai_family = AF_INET;           // IPv4
    hints.ai_socktype = SOCK_STREAM;     // TCP
    hints.ai_protocol = IPPROTO_TCP;     // Protocolo TCP

    if (getaddrinfo(SERVER_ADDRESS, nullptr, &hints, &result) != 0) {
        std::cerr << "Error al resolver la dirección del servidor\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Copiar la dirección resuelta en serverAddr
    memcpy(&serverAddr, result->ai_addr, result->ai_addrlen);
    serverAddr.sin_port = htons(PORT); // Asignar el puerto

    freeaddrinfo(result); // Liberar la memoria de getaddrinfo

    // Conectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar con el servicio\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Conexion establecida con el servicio.\n";

    // Recibir la fecha y hora
    int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (valread > 0) {
        std::cout << "Hora y fecha recibidas: " << buffer << "\n";
    }
    else {
        std::cerr << "Error al recibir datos del servidor\n";
    }

    // Cerrar el socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
