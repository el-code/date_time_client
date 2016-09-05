#include <QCoreApplication>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 2)
    {
        std::cout << "Usage: date_time_client <IpAddress>" << std::endl;
        return 1;
    }

    const size_t MAX_SIZE = 512;
    int sockfd, n;
    char recvline[MAX_SIZE+1];
    sockaddr_in sock_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "socket error" << std::endl;
        return 1;
    }

    bzero(&sock_addr, sizeof(sockaddr_in));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(13);

    if(inet_pton(AF_INET, argv[1], &sock_addr.sin_addr) <= 0)
    {
        std::cerr << "inet_pton error for " << argv[1] << std::endl;
        return 1;
    }

    if(connect(sockfd, (sockaddr*)(&sock_addr), sizeof(sock_addr)) < 0)
    {
        std::cout << "connect failed" << std::endl;
        return 0;
    }

    std::cout << "connected" << std::endl;

    while((n = read(sockfd, recvline, MAX_SIZE)) > 0)
    {
        recvline[n] = '\0';
        std::cout << "Server answer: " << recvline << std::endl;
    }

    if(n < 0)
    {
        std::cerr << "error while reading" << std::endl;
        return 1;
    }


    return a.exec();
}

