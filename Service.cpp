#include <iostream>
#include <winsock2.h>
#include <ctime>

#pragma comment(lib, "ws2_32.lib") // Librería de Winsock

#define PORT 9000
#define BACKLOG 5

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock\n";
        return -1;
    }

    // Crear el socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket\n";
        WSACleanup();
        return -1;
    }

    // Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Enlazar el socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al enlazar el socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Escuchar conexiones
    if (listen(serverSocket, BACKLOG) == SOCKET_ERROR) {
        std::cerr << "Error al escuchar en el puerto\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Servicio iniciado. Escuchando en el puerto " << PORT << "...\n";

    while (true) {
        // Aceptar una conexión del cliente
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
            std::cerr << "Error al aceptar la conexión\n";
            continue;
        }

        std::cout << "Conexion aceptada desde el cliente\n";

        // Obtener la fecha y hora actuales
        std::time_t now = std::time(nullptr);
        char buffer[26]; // ctime_s requiere un búfer de al menos 26 caracteres
        ctime_s(buffer, sizeof(buffer), &now);
        std::string datetime = buffer;

        // Enviar la fecha y hora al cliente
        send(clientSocket, datetime.c_str(), datetime.length(), 0);
        std::cout << "Hora y fecha enviadas: " << datetime;

        // Cerrar la conexión con el cliente
        closesocket(clientSocket);

        // Imprimir el mensaje de "Esperando nueva conexión" después de cada interacción
        std::cout << "Esperando una nueva conexion...\n";
    }

    // Cerrar el socket del servidor y limpiar Winsock
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
