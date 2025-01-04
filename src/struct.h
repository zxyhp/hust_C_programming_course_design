#ifndef STRUCT_H
#define STRUCT_H

#include <iostream>
#include <iomanip>

using namespace std;

// 消费记录语句链表/操作日志语句链表
typedef struct xfStr{
    xfStr(){
        next = nullptr;
    }
    xfStr(string s){
        next = nullptr;
        str = s;
    }
    string str;
    xfStr *next;
}xfStr,opLogStr;

typedef struct {
    opLogStr *first;
    opLogStr *last;
}opLogHead;

// 消费窗口的60000条消费记录的记录结点
typedef struct winXfStr{
    winXfStr(){
        next = nullptr;
        previous = nullptr;
    }
    string cardNum; // 消费卡号
    string stuNum; // 消费学号
    string date; // 消费日期
    string time; // 消费时间
    string name; // 姓名
    float money; // 消费金额
    winXfStr* next; //下一个结点
    winXfStr* previous; // 上一个结点
}winXfStr;

// 分析中有序的消费记录的节点
typedef struct sortedXf{
    sortedXf(){
        next = nullptr;
    }
    sortedXf(string cN,string sN,string d,string t,float m,string n,int num){
        next = nullptr;
        cardNum = cN;
        stuNum = sN;
        date = d;
        time = t;
        money = m;
        name = n;
        winNum = num;
    }
    string cardNum; // 消费卡号
    string stuNum; // 消费学号
    string date; // 消费日期
    string time; // 消费时间
    string name; // 姓名
    float money; // 消费金额
    int winNum; // 消费窗口号
    sortedXf* next; //下一个结点
}sortedXf;

// 相当于头节点
typedef struct winXfHead{
    winXfHead(){
        now = nullptr;
        num = 0;
    }
    winXfStr* now; // 下一个填写位置所对应的指针
    int num; // 整个双向循环链表中有效的记录数 最大为60000
}winXfHead;

// 学生卡号链表
typedef struct CardNum{
    string number;
    CardNum *next;
}CardNum;

// 学生结构体 唯一标志是学号
typedef struct Student{
    string stuNumber;  //学号
    string name;  //姓名
    string password;  //学生校园卡密码

    // 当前正在使用的卡是他的第几张卡（若>100则无法再进行补卡）
    int numOfCard;

    // 该学生名下现今是否有正在使用中的卡
    bool hasCard;

    // 历代使用过的卡号链表 之后的卡用头插法插入
    // 总保持正在使用中或最后一张挂失的卡是首结点
    CardNum *cardhead;

    //学生账户余额 钱记在学生账户名下，不是在卡名下
    // 故即使该用户名下所有卡均已挂失，也可以进行充值
    float money;

    // 学生状态 0:正常状态; 1:已销户
    bool status;
}Student;

// 学生卡 结构体
typedef struct Card{
    string stuNumber; //学号
    string name; //姓名
    string cardNumber; //卡号
    bool status; //当前卡的状态  0:使用中; 1:挂失中;
}Card;

// 食堂窗口结构体
typedef struct Win{
    int consumeTimes; // 当日消费总次数
    float consumeMoney; // 当日消费总金额
    int fileLine; //最近一次消费记录的文件保存位置
}Win;

#endif // STRUCT_H
