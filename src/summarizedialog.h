#ifndef SUMMARIZEDIALOG_H
#define SUMMARIZEDIALOG_H

#pragma once
#include <QDialog>
#include "canteenopration.h"
#include "statisticsdialog.h"
#include "searchdialog.h"
#include "analyzedialog.h"
#include "checkfiledialog.h"

namespace Ui {
class SummarizeDialog;
}

class SummarizeDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：SummarizeDialog
    // 功能：SummarizeDialog类的构造函数，完成成员变量m_COP的初始化，
    //      并对食堂窗口的消费记录完成排序并写入数据文件
    // 输入参数：食堂应用操作类的指针；父窗口部件的指针（没有父窗口则会传
    //          入nullptr）
    // 返回值：空
    //===========================================================
    explicit SummarizeDialog(CanteenOpration* COP, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~SummarizeDialog
    // 功能：SummarizeDialog类的析构函数，在SummarizeDialog类的对象
    //      释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~SummarizeDialog();

    //===========================================================
    // 函数名：getCheckStr
    // 功能：对传入的字符串进行加密，并返回结果
    // 输入参数：加密的起始序号；待加密的语句
    // 返回值：加密后的字符串
    //===========================================================
    static string getCheckStr(int num,string str);

private slots:

    //===========================================================
    // 函数名：on_backButton_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_backButton_clicked();

    //===========================================================
    // 函数名：on_statistButton_clicked
    // 功能：该类窗口界面上的统计按钮的click()信号的槽函数，显示统计窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_statistButton_clicked();

    //===========================================================
    // 函数名：on_pushButton_clicked
    // 功能：该类窗口界面上的查询按钮的click()信号的槽函数，显示查询窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_pushButton_clicked();

    //===========================================================
    // 函数名：on_analyzeButton_clicked
    // 功能：该类窗口界面上的分析按钮的click()信号的槽函数，显示分析窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_analyzeButton_clicked();

    //===========================================================
    // 函数名：on_checkButton_clicked
    // 功能：该类窗口界面上的校验按钮的click()信号的槽函数，显示校验窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_checkButton_clicked();

private:
    Ui::SummarizeDialog *ui;

    // 对应经过归并排序后有序(从头至尾按时间升序)的xf操作
    sortedXf* m_sortedRecords;

    // 对应获取到的COP
    CanteenOpration* m_COP;

    // map<窗口号,pair<记录条数,内容指针>>
    map<int,pair<int,winXfStr*>> m_allReacords;

    // 对应的写入消费记录数据文件的密文的明文字符串链表
    xfStr* m_sortXfStr;

    // 初始化m_allReacords
    void init();

    //===========================================================
    // 函数名：insert_vec
    // 功能：利用折半插入排序，每次插入一个元素到有序序列中(按照str升序)
    // 输入参数：窗口号-1，日期时间字符串，有序待插入容器
    // 返回值：空
    //===========================================================
    void insert_vec(int i, string str, vector<pair<int, string> > &vec);

    //===========================================================
    // 函数名：sortWinXf
    // 功能：用多路归并将传入参数内的信息按日期时间进行排序，并将排序结果存进
    //      m_sortedRecords为头节点的链表中
    // 输入参数：待排序的多路信息的键值对集合
    // 返回值：空
    //===========================================================
    void sortWinXf(map<int,pair<int,winXfStr*>> allReacords);

    //===========================================================
    // 函数名：writeWinXf
    // 功能：将有序链表中的信息结合成语句，并加密后写入数据文件all_xf.txt中
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void writeWinXf();
};

#endif // SUMMARIZEDIALOG_H
