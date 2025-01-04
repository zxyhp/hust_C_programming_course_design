#ifndef WINDOWDIALOG_H
#define WINDOWDIALOG_H

#pragma once
#include <QDialog>
#include <QTimer>
#include "cosumelogdialog.h"
#include "cosumedialog.h"

namespace Ui {
class WindowDialog;
}

class WindowDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：WindowDialog
    // 功能：WindowDialog类的构造函数，初始化成员变量m_winNu，同时完成
    //      时间日期组件(dateTimeEdit)的初始化及其更新的设置
    // 输入参数：窗口号-1；时间日期变量；父窗口部件的指针（没有父窗口则
    //          会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit WindowDialog(int winNum, QDateTime dt,QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~WindowDialog
    // 功能：WindowDialog类的析构函数，在WindowDialog类的对象释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~WindowDialog();

private slots:

    //===========================================================
    // 函数名：on_pushButton_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_pushButton_clicked();

    //===========================================================
    // 函数名：on_consumeButton_clicked
    // 功能：该类窗口界面上的消费按钮的click()信号的槽函数，打开消费窗口
    //      界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_consumeButton_clicked();

    //===========================================================
    // 函数名：on_csLogButton_clicked
    // 功能：该类窗口界面上的查看消费日志按钮的click()信号的槽函数，打开
    //      消费日志窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_csLogButton_clicked();

    //===========================================================
    // 函数名：timeUpdate
    // 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加1ms
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void timeUpdate();

private:
    Ui::WindowDialog *ui;
    int m_winNum;

    //===========================================================
    // 函数名：updateLCDNum
    // 功能：更新窗口界面上显示的窗口当日消费总次数和总金额的示数
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void updateLCDNum();
};

#endif // WINDOWDIALOG_H
