// BSIF 6Gen1 - 基本接口工程第六代
// 网络通信基本实验程序
// 服务器端代码
#include <iostream>
#include <string>
#include <winsock2.h>

int main() {

    // 设置控制台代码页
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    // 初始化Winsock库
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // 创建套接字
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // 设置服务器地址
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // 设置端口号
    serverAddress.sin_addr.s_addr = inet_addr("192.168.77.12");
    
    // 绑定套接字到服务器地址
    bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    
    // 监听连接请求
    listen(serverSocket, SOMAXCONN);
    
    // 接受客户端连接
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    
    // 接收和发送数据
    char buffer[1024];
    std::string message;
    
    while (true) {
        // 接收客户端消息
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "客户端: " << buffer << std::endl;
        
        // 发送回复消息
        std::cout << "回复消息: ";
        std::getline(std::cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
    }
    
    // 关闭套接字
    closesocket(clientSocket);
    closesocket(serverSocket);
    
    // 清理Winsock库
    WSACleanup();
    
    return 0;
}