#ifndef ANALYZEDIALOG_H
#define ANALYZEDIALOG_H

#include <QDialog>
#include "cardoprotion.h"

namespace Ui {
class AnalyzeDialog;
}

class AnalyzeDialog : public QDialog
{
    Q_OBJECT

public:
    //===========================================================
    // 函数名：AnalyzeDialog
    // 功能：AnalyzeDialog类的构造函数，初始化成员变量m_allReacords、
    //      m_sortedRecords，并设置学号输入框的输入限制
    // 输入参数：各个窗口的消费记录信息的入口键值对集合；有序消费记录链表的
    //         头节点的指针；父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit AnalyzeDialog(map<int,pair<int,winXfStr*>> m, sortedXf* sortedRecords, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~AnalyzeDialog
    // 功能：AnalyzeDialog类的析构函数，在AnalyzeDialog类的对象释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~AnalyzeDialog();

private slots:
    //===========================================================
    // 函数名：on_backBtn_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_backBtn_clicked();

    //===========================================================
    // 函数名：on_analyzeBtn_clicked
    // 功能：该类窗口界面上的分析按钮的click()信号的槽函数，分析出与输入
    //      学号对应学生经常一起在食堂排序消费的学生，将他们的信息存入成
    //      员变量m_info中
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_analyzeBtn_clicked();

private:
    Ui::AnalyzeDialog *ui;
    // 食堂窗口的所有记录
    // <窗口号，<数据个数，窗口下一条信息的写入位置>>
    map<int,pair<int,winXfStr*>> m_allReacords;
    // 记录与某位同学经常一起排队的同学信息
    // <学号，<次数，姓名> >
    map<string,pair<int,string>> m_info;
    // 所有窗口记录的有序集合链表头
    sortedXf* m_sortedRecords;

    //===========================================================
    // 函数名：updateTable
    // 功能：更新成员变量m_info的信息至窗口界面的tableWidget组件上
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void updateTable();
};

#endif // ANALYZEDIALOG_H
