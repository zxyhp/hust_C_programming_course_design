#ifndef CANTEENOPRATION_H
#define CANTEENOPRATION_H
#define WINNUM 99
#define FILELINES 60000
#define MAXSIZE 64

#pragma once

#include "cardoprotion.h"
#include "passworddialog.h"
#include <QObject>
#include <fstream>

class CanteenOpration : public QObject
{
    Q_OBJECT

public:
    static CanteenOpration *ptrCOP;
    static CanteenOpration *getinstance(){
        if(nullptr == ptrCOP)
        {
            ptrCOP = new CanteenOpration;
        }
        return ptrCOP;
    }

    //===========================================================
    // 函数名：CanteenOpration
    // 功能：CanteenOpration类的构造函数，初始化部分成员变量
    // 输入参数：空
    // 返回值：空
    //===========================================================
    CanteenOpration();

    //===========================================================
    // 函数名：split
    // 功能：以delim为间隔，将传入的str字符串拆分成若干个字符串
    // 输入参数：待切割的字符串；切割字符串的分隔字符串
    // 返回值：由str切割而成的若干个字符串的集合
    //===========================================================
    static vector<string> split(const string &str, const string &delim);

    //===========================================================
    // 函数名：consume
    // 功能：实现传入卡号对应学生的消费操作
    // 输入参数：是否为批量操作的标志；窗口号-1；卡号；消费金额；日期；时间
    // 返回值：空
    //===========================================================
    void consume(bool isPl, int winNum, string cardNum, float conMoney, string date, string time);

    //===========================================================
    // 函数名：getWinConsume
    // 功能：用于类外获取私有成员变量窗口中记录的消费总次数和消费总金额
    // 输入参数：窗口号-1
    // 返回值：<窗口的当日总消费次数，窗口的当日总消费金额>
    //===========================================================
    pair<int,float> getWinConsume(int winnum);

    //===========================================================
    // 函数名：getWinLine
    // 功能：用于类外获取私有成员变量窗口的新写入行数
    // 输入参数：窗口号-1
    // 返回值：窗口消费记录的下一条记录的写入行数
    //===========================================================
    int getWinLine(int winnum);

    //===========================================================
    // 函数名：getXfRecords
    // 功能：用于类外获取某个窗口的消费记录的入口
    // 输入参数：窗口号-1
    // 返回值：窗口的消费记录的入口变量
    //===========================================================
    winXfHead getXfRecords(int num);

private slots:
    //===========================================================
    // 函数名：receiveData
    // 功能：用于接收密码输入窗口中输入框内输入的字符串至成员变量m_password中
    // 输入参数：密码输入窗口中输入框内输入的字符串
    // 返回值：空
    //===========================================================
    void receiveData(QString data);

private:
    // 99个窗口
    Win win[WINNUM];
    string m_date; // 日期
    string m_time; // 时间
    char m_timeFrame; // 时段 1,早上; 2,中午; 3,晚上
    // 保存0-98每个窗口消费记录的容器 <窗口号，记录链表入口对象>
    map<int,winXfHead> m_winXfRecords;
    // 从密码窗口获取的输入的密码
    QString m_password;
    // 记录某时段某学号的消费金额(与密码的输入与否有关)
    map<string,float> m_csMoneyInFrame;

    //===========================================================
    // 函数名：initWinXfRecords
    // 功能：初始化成员变量m_winXfRecords[0-98]（99个窗口的消费记录入口）
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void initWinXfRecords();

    //===========================================================
    // 函数名：initWin
    // 功能：初始化成员变量m_win[0-98]（99个窗口）
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void initWin();

    //===========================================================
    // 函数名：updateWin
    // 功能：对99个窗口的当日消费总金额和总次数清零
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void updateWin();

    //===========================================================
    // 函数名：updateTime
    // 功能：更新成员变量m_date和m_time
    // 输入参数：是否为批量操作的标志；新日期；新时间
    // 返回值：日期时间更新成功与否
    //===========================================================
    bool updateTime(bool isPl, string date,string time);

    //===========================================================
    // 函数名：updateTFrame
    // 功能：根据传入的时间更新类的时段变量m_timeFrame
    // 输入参数：更新后的新时间
    // 返回值：空
    //===========================================================
    void updateTFrame(int time);

    //===========================================================
    // 函数名：writeXfRecords
    // 功能：将一条消费记录信息写入到对应窗口的消费记录链表的对应节点中
    // 输入参数：窗口号-1；卡号；学号；日期；时间；消费金额
    // 返回值：空
    //===========================================================
    void writeXfRecords(int winNum,string cardNum
              ,string stuNum,string date,string time,float conMoney);
};

#endif // CANTEENOPRATION_H
