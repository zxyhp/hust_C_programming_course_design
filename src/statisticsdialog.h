#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#pragma once
#include <QDialog>
#include "cardoprotion.h"

namespace Ui {
class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：StatisticsDialog
    // 功能：StatisticsDialog类的构造函数，完成成员变量m_sortedRecords
    //      的初始化
    // 输入参数：有序消费日志语句的链表头节点指针；父窗口部件的指针（没
    //          有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit StatisticsDialog(sortedXf *xf, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~StatisticsDialog
    // 功能：StatisticsDialog类的析构函数，在StatisticsDialog类的对象
    //      释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~StatisticsDialog();

private slots:

    //===========================================================
    // 函数名：on_backButton_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_backButton_clicked();

    //===========================================================
    // 函数名：on_inquireButton_clicked
    // 功能：该类窗口界面上的查询按钮的click()信号的槽函数，遍历消费信息
    //      链表根据限定日期和学号，计算学生在该段时间的消费次数和金额并
    //      显示在窗口界面的组件上
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_inquireButton_clicked();

private:
    Ui::StatisticsDialog *ui;
    sortedXf *m_sortedRecords;
};

#endif // STATISTICSDIALOG_H
