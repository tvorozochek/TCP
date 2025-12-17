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
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&addr, sizeof(addr)) != 0)
    {
        cout << "Ошибка подключения\n";
        return 1;
    }

    cout << "Подключено к серверу\n";

    while (true)
    {
        Student st{};

        cout << "\nВведите ФИО (exit для выхода): ";
        cin.getline(st.name, 64);
        if (string(st.name) == "exit")
            break;

        cout << "Введите факультет: ";
        cin.getline(st.faculty, 64);

        cout << "Введите 4 оценки: ";
        for (int i = 0; i < 4; i++)
            cin >> st.marks[i];
        cin.ignore();

        send(clientSocket, (char*)&st, sizeof(st), 0);

        Scholarship result;
        recv(clientSocket, (char*)&result, sizeof(result), 0);

        if (result == NO)
            cout << "Стипендия: нет\n";
        else if (result == ORDINARY)
            cout << "Стипендия: обычная\n";
        else
            cout << "Стипендия: повышенная\n";
    }

    closesocket(clientSocket);
    WSACleanup();
}
