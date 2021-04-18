//
// Created by king_ on 2021/4/18.
//

#include "MyServer.h"
#include "NetMessage.h"
#include "CellLog.h"
#include "CellMsgStream.h"

//TODO::拆分
void MyServer::OnNetMsg(CELLServer *pServer, CELLClient *pClient, netmsg_DataHeader *header) {
    EasyTcpServer::OnNetMsg(pServer, pClient, header);
    switch (header->cmd) {
        case CMD_LOGIN: {
            pClient->resetDTHeart();
            //send recv
            // netmsg_Login* login = (netmsg_Login*)header;
            //忽略判断用户密码是否正确的过程
            netmsg_LoginR ret;
            if (SOCKET_ERROR == pClient->SendData(&ret)) {
                //发送缓冲区满了，消息没发出去
                CELLLog::Info("<Socket=%d> Send Full\n", pClient->sockfd());
            }
        }//接收 消息---处理 发送   生产者 数据缓冲区  消费者
            break;
        case CMD_LOGOUT: {
            CELLReadStream r(header);
            //读取消息长度
            r.ReadInt16();
            //读取消息命令
            r.getNetCmd();
            auto n1 = r.ReadInt8();
            auto n2 = r.ReadInt16();
            auto n3 = r.ReadInt32();
            auto n4 = r.ReadFloat();
            auto n5 = r.ReadDouble();
            uint32_t n = 0;
            r.onlyRead(n);
            char name[32] = {};
            auto n6 = r.ReadArray(name, 32);
            char pw[32] = {};
            auto n7 = r.ReadArray(pw, 32);
            int ata[10] = {};
            auto n8 = r.ReadArray(ata, 10);
            ///
            CELLWriteStream s(128);
            s.setNetCmd(CMD_LOGOUT_RESULT);
            s.WriteInt8(n1);
            s.WriteInt16(n2);
            s.WriteInt32(n3);
            s.WriteFloat(n4);
            s.WriteDouble(n5);
            s.WriteArray(name, n6);
            s.WriteArray(pw, n7);
            s.WriteArray(ata, n8);
            s.finsh();
            pClient->SendData(s.data(), s.length());
        }
            break;
        case CMD_C2S_HEART: {
            pClient->resetDTHeart();
            netmsg_s2c_Heart ret;
            pClient->SendData(&ret);
        }
        default: {
            CELLLog::Info("recv <socket=%d> undefine msgType,dataLen：%d\n", pClient->sockfd(), header->dataLength);
        }
            break;
    }
}