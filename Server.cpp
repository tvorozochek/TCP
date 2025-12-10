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

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "Ошибка bind\n";
        return 1;
    }

    listen(serverSocket, SOMAXCONN);
    cout << "Ожидание клиента...\n";

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Клиент подключён!\n";

    char nameBuf[128];
    char facBuf[128];
    int marks[4]{};

    recv(clientSocket, nameBuf, sizeof(nameBuf), 0);
    recv(clientSocket, facBuf, sizeof(facBuf), 0);
    recv(clientSocket, (char*)marks, sizeof(marks), 0);

    cout << "ФИО: " << nameBuf << endl;
    cout << "Факультет: " << facBuf << endl;
    cout << "Оценки: ";
    for (int i = 0; i < 4; i++) cout << marks[i] << " ";
    cout << endl;

    int sum = 0;
    bool hasLow = false;

    for (int i = 0; i < 4; i++)
    {
        sum += marks[i];
        if (marks[i] <= 3) hasLow = true;
    }

    int result = 0;
    if (hasLow) result = 0;
    else if (sum == 20) result = 2;
    else result = 1;

    string response;

    if (result == 0) response = "Степендия: нет";
    if (result == 1) response = "Степендия: обычная";
    if (result == 2) response = "Степендия: повышенная";

    send(clientSocket, response.c_str(), response.size() + 1, 0);

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
