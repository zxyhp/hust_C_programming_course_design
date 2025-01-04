#ifndef STRUCT_H
#define STRUCT_H

#include <iostream>
#include <iomanip>

using namespace std;

// ���Ѽ�¼�������/������־�������
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

// ���Ѵ��ڵ�60000�����Ѽ�¼�ļ�¼���
typedef struct winXfStr{
    winXfStr(){
        next = nullptr;
        previous = nullptr;
    }
    string cardNum; // ���ѿ���
    string stuNum; // ����ѧ��
    string date; // ��������
    string time; // ����ʱ��
    string name; // ����
    float money; // ���ѽ��
    winXfStr* next; //��һ�����
    winXfStr* previous; // ��һ�����
}winXfStr;

// ��������������Ѽ�¼�Ľڵ�
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
    string cardNum; // ���ѿ���
    string stuNum; // ����ѧ��
    string date; // ��������
    string time; // ����ʱ��
    string name; // ����
    float money; // ���ѽ��
    int winNum; // ���Ѵ��ں�
    sortedXf* next; //��һ�����
}sortedXf;

// �൱��ͷ�ڵ�
typedef struct winXfHead{
    winXfHead(){
        now = nullptr;
        num = 0;
    }
    winXfStr* now; // ��һ����дλ������Ӧ��ָ��
    int num; // ����˫��ѭ����������Ч�ļ�¼�� ���Ϊ60000
}winXfHead;

// ѧ����������
typedef struct CardNum{
    string number;
    CardNum *next;
}CardNum;

// ѧ���ṹ�� Ψһ��־��ѧ��
typedef struct Student{
    string stuNumber;  //ѧ��
    string name;  //����
    string password;  //ѧ��У԰������

    // ��ǰ����ʹ�õĿ������ĵڼ��ſ�����>100���޷��ٽ��в�����
    int numOfCard;

    // ��ѧ�������ֽ��Ƿ�������ʹ���еĿ�
    bool hasCard;

    // ����ʹ�ù��Ŀ������� ֮��Ŀ���ͷ�巨����
    // �ܱ�������ʹ���л����һ�Ź�ʧ�Ŀ����׽��
    CardNum *cardhead;

    //ѧ���˻���� Ǯ����ѧ���˻����£������ڿ�����
    // �ʼ�ʹ���û��������п����ѹ�ʧ��Ҳ���Խ��г�ֵ
    float money;

    // ѧ��״̬ 0:����״̬; 1:������
    bool status;
}Student;

// ѧ���� �ṹ��
typedef struct Card{
    string stuNumber; //ѧ��
    string name; //����
    string cardNumber; //����
    bool status; //��ǰ����״̬  0:ʹ����; 1:��ʧ��;
}Card;

// ʳ�ô��ڽṹ��
typedef struct Win{
    int consumeTimes; // ���������ܴ���
    float consumeMoney; // ���������ܽ��
    int fileLine; //���һ�����Ѽ�¼���ļ�����λ��
}Win;

#endif // STRUCT_H
