#ifndef CARDOPROTION_H
#define CARDOPROTION_H

#pragma once

#include"struct.h"
#include"oprationlog.h"
#include<sstream>
#include<map>

#define MAXMONEY 1000  //充值金额上限

using namespace std;

// 校园卡管理的模拟系统
class CardOpration{
public:
    static CardOpration *ptrCO;
    static CardOpration *getinstance(){
        if(nullptr == ptrCO)
        {
            ptrCO = new CardOpration;
        }
        return ptrCO;
    }

    //===========================================================
    // 函数名：CardOpration
    // 功能：CardOpration类的构造函数，初始化成员变量cardNum(流水卡号)
    // 输入参数：空
    // 返回值：空
    //===========================================================
    CardOpration();

    //===========================================================
    // 函数名：createStu
    // 功能：实现学生的开户功能
    // 输入参数：日期；时间；学号；姓名
    // 返回值：空
    //===========================================================
    void createStu(string date,string time,string studentNum, string name);

    //===========================================================
    // 函数名：deleteStu
    // 功能：实现学生的销户功能
    // 输入参数：日期；时间；学号
    // 返回值：空
    //===========================================================
    void deleteStu(string date, string time, string studentNum);

    //===========================================================
    // 函数名：lossSolution_1
    // 功能：对学生校园卡的挂失功能
    // 输入参数：日期；时间；学号
    // 返回值：空
    //===========================================================
    void lossSolution_1(string date, string time, string studentNum);

    //===========================================================
    // 函数名：lossSolution_2
    // 功能：对学生校园卡的解挂功能
    // 输入参数：日期；时间；学号
    // 返回值：空
    //===========================================================
    void lossSolution_2(string date, string time, string studentNum,string cardNum = "");

    //===========================================================
    // 函数名：makeUpCard
    // 功能：对学生校园卡的补卡功能
    // 输入参数：日期；时间；学号
    // 返回值：空
    //===========================================================
    void makeUpCard(string date, string time, string studentNum);

    //===========================================================
    // 函数名：recharge
    // 功能：对学生校园卡的充值功能
    // 输入参数：日期；时间；学号；充值金额
    // 返回值：空
    //===========================================================
    void recharge(string date, string time, string studentNum, float addMoney);

    map<string,Student> students; //开过户的学生 <学号，学生>
    map<string,Card> cards; //发放过的学生卡 <卡号，卡>

private:
    string cardNum; //当前已经发到的学生卡号
    OprationLog oplog;

    //===========================================================
    // 函数名：stuExist
    // 功能：判断传入学号是否存在于students中
    // 输入参数：学号
    // 返回值：判断结果，false则不存在，true则存在
    //===========================================================
    bool stuExist(string studentNum);

    //===========================================================
    // 函数名：stuLeave
    // 功能：判断传入学号对应的学生状态
    // 输入参数：学号
    // 返回值：判断结果，true则已注销，false则状态正常
    //===========================================================
    bool stuLeave(string studentNum);

    //===========================================================
    // 函数名：addCard
    // 功能：向对应学号的学生名下添加一张卡片，并将该卡添加到学生信息系统的
    //      cards容器中
    // 输入参数：学号
    // 返回值：被发放卡片的卡号
    //===========================================================
    string addCard(string studentNum);

    //===========================================================
    // 函数名：putUpCards
    // 功能：对新开户的学生进行发卡的功能
    // 输入参数：日期；时间；学号
    // 返回值：空
    //===========================================================
    void putUpCards(string date, string time, string studentNum);
};

#endif // CARDOPROTION_H
