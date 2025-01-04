#ifndef CANTEENAPPDIALOG_H
#define CANTEENAPPDIALOG_H

#pragma once
#include <QDialog>
#include "windowdialog.h"
#include "setdatetime.h"

namespace Ui {
class CanteenAppDialog;
}

class CanteenAppDialog : public QDialog
{
    Q_OBJECT

public:
    //===========================================================
    // 函数名：CanteenAppDialog
    // 功能：CanteenAppDialog类的构造函数，完成时间日期组件(dateTimeEdit)
    //      的初始化，及其更新的设置
    // 输入参数：时间日期变量；父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit CanteenAppDialog(QDateTime dt, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~CanteenAppDialog
    // 功能：CanteenAppDialog类的析构函数，在CanteenAppDialog类的对象
    //      释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~CanteenAppDialog();

private slots:

    //===========================================================
    // 函数名：on_goBackButton_clicked
    // 功能：该类窗口界面上的返回按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_goBackButton_clicked();

    //===========================================================
    // 函数名：on_winTable_cellDoubleClicked
    // 功能：该类窗口界面上的winTable组件的单元格双击信号的槽函数，创建并
    //      打开单元格对应的食堂消费窗口界面
    // 输入参数：被选中单元格的行下标，被选中单元格的列下标
    // 返回值：空
    //===========================================================
    void on_winTable_cellDoubleClicked(int row, int column);

    //===========================================================
    // 函数名：timeUpdate
    // 功能：更新窗口界面中的dateTimeEdit组件的显示日期时间，每次更新加1ms
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void timeUpdate();

private:
    Ui::CanteenAppDialog *ui;
};

#endif // CANTEENAPPDIALOG_H
