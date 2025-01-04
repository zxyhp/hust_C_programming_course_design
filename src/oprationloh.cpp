#include "oprationlog.h"

opLogHead *OprationLog::opLog = nullptr;

//===========================================================
// 函数名：writeOpLog
// 功能：用尾插法将一条操作日志加入到记录操作日志的链表的尾部
// 输入参数：一条操作日志语句
// 返回值：空
//===========================================================
void OprationLog::writeOpLog(string str){
    auto oploghead = OprationLog::getinstance();

    // 构建存放日志语句的新节点
    opLogStr *op = new opLogStr;
    op->str = str;
    op->next = nullptr;

    // 将该节点加入链表尾部
    oploghead->last->next = op;
    // 更新链表尾部
    oploghead->last = op;
}
