#include <csignal>
#include <cstring>

#include <iostream>
#include <thread>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

#define BUF_SIZE 10*1024

bool isQuit = false;
bool isExit = false;
void handler(int param)
{
    isQuit = true;
    isExit = true;
}

void worker()
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(2000);

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        cout << "create socket failed!" << endl;
        return;
    }

    int opt = 1;
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        cout << "bind addr failed!" << endl;
        return;
    }

    if (::listen(fd, 5) == -1)
    {
        cout << "listen failed!" << endl;
        return;
    }

    while (!isExit)
    {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

        int client_fd = accept(fd, (struct sockaddr*)&client_addr, &length);
        if (client_fd < 0)
        {
            cout << "server accept failed!" << endl;
            break;
        }

        char buffer[BUF_SIZE];
        bzero(buffer, BUF_SIZE);
        length = ::recv(client_fd, buffer, BUF_SIZE, 0);
        cout << buffer << endl;
    }
}

int main(int argc, char ** argv)
{
    signal(SIGINT, handler);
    thread worker_id(worker);
    worker_id.detach();

    while (!isQuit)
    {
        string common;
        cin >> common;
        cout << common << endl;
    }    
}