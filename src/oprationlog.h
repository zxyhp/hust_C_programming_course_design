#ifndef OPRATIONLOG_H
#define OPRATIONLOG_H
#include "struct.h"

#pragma once

class OprationLog
{
public:
    static opLogHead *opLog;
    static opLogHead *getinstance(){
        if(nullptr == opLog){
            opLog = new opLogHead;
            opLog->first = new opLogStr;
            opLog->first->next = nullptr;
            opLog->last = opLog->first;
        }
        return opLog;
    }

    //===========================================================
    // 函数名：writeOpLog
    // 功能：用尾插法将一条操作日志加入到记录操作日志的链表的尾部
    // 输入参数：一条操作日志语句
    // 返回值：空
    //===========================================================
    void writeOpLog(string str);
};

#endif // OPRATIONLOG_H
