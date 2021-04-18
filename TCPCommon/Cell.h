//
// Created by king_ on 2021/4/18.
//

#ifndef TEST_CELL_H
#define TEST_CELL_H

//SOCKET
#ifdef _WIN32
    #define FD_SETSIZE      256
    #define WIN32_LEAN_AND_MEAN
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include<Windows.h>
    #include<WinSock2.h>
    #define EXPORT_DLL _declspec(dllexport)
#else

    #include<unistd.h> //uni std
    #include<arpa/inet.h>
    #include<string.h>
    #include<signal.h>
    #include<sys/socket.h>

    #define SOCKET int
    #define INVALID_SOCKET  (SOCKET)(~0)
    #define SOCKET_ERROR            (-1)
    #define EXPORT_DLL
#endif

#include <cstdio>
#include <cstdint>

//缓冲区最小单元大小
#ifndef RECV_BUFF_SZIE
#define RECV_BUFF_SZIE 8192
#define SEND_BUFF_SZIE 10240
#endif // !RECV_BUFF_SZIE

#endif //TEST_CELL_H
