#include <iostream>
#include <string>
#include <winsock2.h>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    // 初始化Winsock库
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // 创建套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // 设置服务器地址
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // 设置服务器端口号
    serverAddress.sin_addr.s_addr = inet_addr("192.168.77.12");
    
    // 连接到服务器
    connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    
    // 接收和发送数据
    char buffer[1024];
    std::string message;
    
    while (true) {
        // 发送消息
        std::cout << "消息: ";
        std::getline(std::cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
        
        // 接收服务器回复
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "服务器: " << buffer << std::endl;
    }
    
    // 关闭套接字
    closesocket(clientSocket);
    
    // 清理Winsock库
    WSACleanup();
    
    return 0;
}