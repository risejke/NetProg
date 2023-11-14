#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
	struct sockaddr_in {
 		short sin_family;
 		unsigned short sin_port;
 		struct in_addr sin_addr;
 		char sin_zero[8];
	};
	struct in_addr {
 		unsigned long s_addr;
	};
    // Создание сокета
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error creating socket\n";
        return 1;
    }

    // Настройка адреса сервера
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);
    serverAddr.sin_addr.s_addr = 0;

    // Привязка сокета к адресу
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error binding socket to address\n";
        close(serverSocket);
        return 1;
    }

    // Перевод сокета в режим прослушивания
    if (listen(serverSocket, 10) == -1) {
        cerr << "Error listening on socket\n";
        close(serverSocket);
        return 1;
    }

    cout << "Server listening on port 7777...\n";

    while (true) {
        // Принятие входящего соединения
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            cerr << "Error accepting connection\n";
            continue;
        }

        cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;

        // Обработка данных от клиента
        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            cout << "Received: " << buffer;
            // Отправка обратно клиенту
            send(clientSocket, buffer, bytesRead, 0);
            // Очистка буфера
            memset(buffer, 0, sizeof(buffer));
        }

        if (bytesRead == 0) {
            // Соединение было закрыто клиентом
            cout << "Client disconnected\n";
        } else if (bytesRead == -1) {
            cerr << "Error receiving data\n";
        }

        // Закрытие сокета клиента
        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}
