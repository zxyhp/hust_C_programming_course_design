#ifndef LEADDATA_H
#define LEADDATA_H
#define WINNUM 99
#define MAXSIZE 64

#include "struct.h"
#include "cardoprotion.h"
#include "setdatetime.h"
#include "canteenopration.h"
#include <sstream>
#include <QDir>
#include <fstream>

class LeadData
{
public:

    //===========================================================
    // ��������LeadData
    // ���ܣ�LeadData��Ĺ��캯������ɳ�Ա����m_sortedOp��m_opration
    //      �ĳ�ʼ��������ʳ�ô��ڵ����Ѽ�¼�������д�������ļ�
    // �����������
    // ����ֵ����
    //===========================================================
    LeadData();

    // ����������������ʽ
    typedef void (LeadData::*Func)();

    //===========================================================
    // ��������runTime
    // ���ܣ����㴫�뺯��������ʱ�䲢��ʾ�ڿ���̨������
    // ���������LeadData��ĺ���ָ��
    // ����ֵ����
    //===========================================================
    void runTime(Func f);

    //===========================================================
    // ��������initStu
    // ���ܣ�����ͬĿ¼��students.txt�ļ��ڵ�ѧ����Ϣ����ѧ����������Ϣ
    //      ���뵽ѧ����Ϣϵͳ��students������
    // �����������
    // ����ֵ����
    //===========================================================
    void initStu();

    //===========================================================
    // ��������getOpration
    // ���ܣ���xf.txt��cz.txt�ļ��л�ȡϵ�в������
    // �����������
    // ����ֵ����
    //===========================================================
    void getOpration();

    //===========================================================
    // ��������sortOpration
    // ���ܣ������в������ù鲢������۰�����������ʱ����Ⱥ�������򣬽�
    //      �����Ľ�����뵽��Ա����m_sortedOp��ָ��������
    // �����������
    // ����ֵ����
    //===========================================================
    void sortOpration();

    //===========================================================
    // ��������carryOutOp
    // ���ܣ��������Ĳ�����ѧ��������ʳ��Ӧ��ϵͳ�н��д�����β��ʵʩ
    // �����������
    // ����ֵ�����һ��������<���ڣ�ʱ��>
    //===========================================================
    pair<string,string> carryOutOp();

private:
    // 0-98��Ӧ99�����ڵ����Ѳ���(xf.txt)
    // 99��Ӧ��Ƭ�Ĳ�����ʧ��ֵ�Ȳ���(cz.txt)
    xfStr* m_opration[WINNUM+1];
    // ��Ӧ�����op����
    xfStr* m_sortedOp;

    //===========================================================
    // ��������init_m_opration
    // ���ܣ���ʼ����Ա����m_opration
    // �����������
    // ����ֵ����
    //===========================================================
    void init_m_opration();

    //===========================================================
    // ��������insert_vec
    // ���ܣ������۰��������ÿ�β���һ��Ԫ�ص�����������(����str����)
    // ������������ں�-1������ʱ���ַ������������������
    // ����ֵ����
    //===========================================================
    void insert_vec(int i, string str, vector<pair<int,string> >& vec);

};

#endif // LEADDATA_H