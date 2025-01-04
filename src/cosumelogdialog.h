#ifndef COSUMELOGDIALOG_H
#define COSUMELOGDIALOG_H

#pragma once

#include <QDialog>
#include "canteenopration.h"

namespace Ui {
class CosumeLogDialog;
}

class CosumeLogDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：CosumeLogDialog
    // 功能：CosumeLogDialog类的构造函数，完成成员变量m_winNum的初始化，
    //      并将对应窗口号的消费记录信息同步到窗口界面的表组件上
    // 输入参数：食堂窗口号-1；父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit CosumeLogDialog(int num, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~CosumeLogDialog
    // 功能：CosumeLogDialog类的析构函数，在CosumeLogDialog类的对象释
    //      放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~CosumeLogDialog();

private slots:

    //===========================================================
    // 函数名：on_goBackButton_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_goBackButton_clicked();

private:
    Ui::CosumeLogDialog *ui;
    int m_winNum;

    //===========================================================
    // 函数名：updateTable
    // 功能：将对应窗口号的消费记录内容显示到窗口界面的tableWidget组件上
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void updateTable();
};

#endif // COSUMELOGDIALOG_H
