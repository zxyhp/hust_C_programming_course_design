#ifndef COSUMEDIALOG_H
#define COSUMEDIALOG_H

#pragma once

#include <QDialog>
#include "canteenopration.h"
#include "setdatetime.h"

namespace Ui {
class CosumeDialog;
}

class CosumeDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：CosumeDialog
    // 功能：CosumeDialog类的构造函数，完成成员变量m_winNum的初始化，
    //      并设置窗口界面的两个输入框的输入内容限制
    // 输入参数：食堂窗口号-1；父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit CosumeDialog(int winNum, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~CosumeDialog
    // 功能：CosumeDialog类的析构函数，在CosumeDialog类的对象释放时
    //      释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~CosumeDialog();

private slots:

    //===========================================================
    // 函数名：on_cancelButton_clicked
    // 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_cancelButton_clicked();

    //===========================================================
    // 函数名：on_okButton_clicked
    // 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据输入信息
    //      调用系统中的消费函数完成学生的消费操作
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_okButton_clicked();

private:
    Ui::CosumeDialog *ui;
    int m_winNum;
};

#endif // COSUMEDIALOG_H
