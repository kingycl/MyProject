//
// Created by king_ on 2021/4/18.
//

#include "MyClient.h"
#include "CellLog.h"

void MyClient::OnNetMsg(netmsg_DataHeader *header) {
    switch (header->cmd) {
        case CMD_LOGIN_RESULT: {
            // netmsg_LoginR* login = (netmsg_LoginR*)header;
            // CELLLog::Info("<socket=%d> recv msgType：CMD_LOGIN_RESULT\n", (int)_pClient->sockfd());
        }
            break;
        case CMD_LOGOUT_RESULT: {
            // netmsg_LogoutR* logout = (netmsg_LogoutR*)header;
            // CELLLog::Info("<socket=%d> recv msgType：CMD_LOGOUT_RESULT\n", (int)_pClient->sockfd());
        }
            break;
        case CMD_NEW_USER_JOIN: {
            // netmsg_NewUserJoin* userJoin = (netmsg_NewUserJoin*)header;
            // CELLLog::Info("<socket=%d> recv msgType：CMD_NEW_USER_JOIN\n", (int)_pClient->sockfd());
        }
            break;
        case CMD_ERROR: {
            CELLLog::Info("<socket=%d> recv msgType：CMD_ERROR\n", (int) _pClient->sockfd());
        }
            break;
        default: {
            CELLLog::Info("error, <socket=%d> recv undefine msgType\n", (int) _pClient->sockfd());
        }
    }
}