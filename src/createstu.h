#ifndef CREATESTU_H
#define CREATESTU_H

#pragma once

#include <QDialog>
#include "cardoprotion.h"
#include "setdatetime.h"

namespace Ui {
class CreateStu;
}

class CreateStu : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：CreateStu
    // 功能：CreateStu类的构造函数，设置窗口界面的两个输入框的输入内容限制
    // 输入参数：父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit CreateStu(QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~CreateStu
    // 功能：CreateStu类的析构函数，在CreateStu类的对象释放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~CreateStu();

private slots:

    //===========================================================
    // 函数名：on_okButton_clicked
    // 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据输入信息
    //      调用系统中的开户函数完成学生的开户
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_okButton_clicked();

    //===========================================================
    // 函数名：on_cancleButton_clicked
    // 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_cancleButton_clicked();

private:
    Ui::CreateStu *ui;
};

#endif // CREATESTU_H
