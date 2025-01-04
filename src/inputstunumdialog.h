#ifndef INPUTSTUNUMDIALOG_H
#define INPUTSTUNUMDIALOG_H

#pragma once

#include <QDialog>
#include "cardoprotion.h"
#include "setdatetime.h"

namespace Ui {
class inPutStuNumDialog;
}

class inPutStuNumDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：inPutStuNumDialog
    // 功能：inPutStuNumDialog类的构造函数，完成成员变量m_choice的初始化，
    //      并根据选择，设置该复用窗口界面的窗口名，同时设置输入框的输入限制
    // 输入参数：选择数；父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit inPutStuNumDialog(int choice, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~inPutStuNumDialog
    // 功能：inPutStuNumDialog类的析构函数，在CosumeDialog类的对象释
    //      放时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~inPutStuNumDialog();

private slots:

    //===========================================================
    // 函数名：on_okButton_clicked
    // 功能：该类窗口界面上的确认按钮的click()信号的槽函数，根据选择变量
    //      和输入信息调用系统中对应的功能函数（m_choice：1,补卡；2,挂失；
    //      3,解挂；4,销户）
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_okButton_clicked();

    //===========================================================
    // 函数名：on_cancelButton_clicked
    // 功能：该类窗口界面上的取消按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_cancelButton_clicked();

private:
    Ui::inPutStuNumDialog *ui;
    int m_choice;
};

#endif // INPUTSTUNUMDIALOG_H
