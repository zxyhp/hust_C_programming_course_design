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
    // ��������writeOpLog
    // ���ܣ���β�巨��һ��������־���뵽��¼������־�������β��
    // ���������һ��������־���
    // ����ֵ����
    //===========================================================
    void writeOpLog(string str);
};

#endif // OPRATIONLOG_H
