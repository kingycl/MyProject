//
// Created by king_ on 2021/6/14.
//

#ifndef COMMMSG_H
#define COMMMSG_H


enum CMD {
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_ERROR
};

struct DataHeader {
    short dataLength;
    short cmd;
};

//DataPackage
struct Login : public DataHeader {
    Login() {
        dataLength = sizeof(Login);
        cmd = CMD_LOGIN;
    }
    char userName[32];
    char PassWord[32];
};

struct LoginResult : public DataHeader {
    LoginResult() {
        dataLength = sizeof(LoginResult);
        cmd = CMD_LOGIN_RESULT;
        result = 0;
    }
    int result;
};

struct Logout : public DataHeader {
    Logout() {
        dataLength = sizeof(Logout);
        cmd = CMD_LOGOUT;
    }
    char userName[32];
};

struct LogoutResult : public DataHeader {
    LogoutResult() {
        dataLength = sizeof(LogoutResult);
        cmd = CMD_LOGIN_RESULT;
        result = 0;
    }
    int result;
};

#endif// COMMMSG_H
