#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (err != 0)
    {
        cout << "WSAStartup failed: " << err << endl;
        return 1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        cout << "Winsock version 2.2 not supported" << endl;
        WSACleanup();
        return 1;
    }

    cout << "Winsock 2.2 initialized\n";

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == 0)
        cout << "Подключено к серверу\n";

    string name, fac;
    int marks[4];

    cout << "Введите ФИО: ";
    getline(cin, name);

    cout << "Введите факультет: ";
    getline(cin, fac);

    cout << "Введите 4 оценки: ";
    for (int i = 0; i < 4; i++)
        cin >> marks[i];

    send(clientSocket, name.c_str(), name.size() + 1, 0);
    send(clientSocket, fac.c_str(), fac.size() + 1, 0);
    send(clientSocket, (char*)marks, sizeof(marks), 0);

    char buffer[128];
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        cout << "От сервера: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
