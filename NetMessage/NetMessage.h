//
// Created by king_ on 2020/11/15.
//

#ifndef TEST_NETMESSAGE_H
#define TEST_NETMESSAGE_H

struct DataHeader {
    short dataLength;
    short cmd;
};

struct DataPackage {
    int age;
    char name[32];
};

#endif//TEST_NETMESSAGE_H
