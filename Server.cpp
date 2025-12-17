#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Student
{
    char name[64];
    char faculty[64];
    int marks[4];
};

enum Scholarship
{
    NO = 0,
    ORDINARY = 1,
    HIGH = 2
};

int main()
{
    setlocale(LC_ALL, "Russian");

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "WSAStartup error\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout << "Bind error\n";
        return 1;
    }

    listen(serverSocket, SOMAXCONN);
    cout << "Сервер запущен...\n";

    while (true)
    {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
            continue;

        cout << "Клиент подключился\n";

        while (true)
        {
            Student st;
            int received = recv(clientSocket, (char*)&st, sizeof(st), 0);
            if (received <= 0)
                break;

            cout << "Запрос:\n";
            cout << "ФИО: " << st.name << endl;
            cout << "Факультет: " << st.faculty << endl;
            cout << "Оценки: ";
            for (int i = 0; i < 4; i++)
                cout << st.marks[i] << " ";
            cout << endl;

            bool hasLow = false;
            int sum = 0;

            for (int i = 0; i < 4; i++)
            {
                if (st.marks[i] <= 3)
                    hasLow = true;
                sum += st.marks[i];
            }

            Scholarship result;
            if (hasLow)
                result = NO;
            else if (sum == 20)
                result = HIGH;
            else
                result = ORDINARY;

            send(clientSocket, (char*)&result, sizeof(result), 0);
        }

        cout << "Клиент отключился\n";
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
}
