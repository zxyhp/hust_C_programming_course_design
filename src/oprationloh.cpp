#include "oprationlog.h"

opLogHead *OprationLog::opLog = nullptr;

//===========================================================
// ��������writeOpLog
// ���ܣ���β�巨��һ��������־���뵽��¼������־�������β��
// ���������һ��������־���
// ����ֵ����
//===========================================================
void OprationLog::writeOpLog(string str){
    auto oploghead = OprationLog::getinstance();

    // ���������־�����½ڵ�
    opLogStr *op = new opLogStr;
    op->str = str;
    op->next = nullptr;

    // ���ýڵ��������β��
    oploghead->last->next = op;
    // ��������β��
    oploghead->last = op;
}
