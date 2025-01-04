#ifndef SHOWSTUDIALOG_H
#define SHOWSTUDIALOG_H

#pragma once
#include <QDialog>
#include "cardoprotion.h"
#include "passworddialog.h"

namespace Ui {
class ShowStuDialog;
}

class ShowStuDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // 函数名：ShowStuDialog
    // 功能：ShowStuDialog类的构造函数，初始化成员变量m_stuNum，根据传
    //      入学生学号更新窗口界面信息
    // 输入参数：学生学号；父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit ShowStuDialog(string stuNum, QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~ShowStuDialog
    // 功能：ShowStuDialog类的析构函数，在ShowStuDialog类的对象释放
    //      时释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~ShowStuDialog();

private slots:

    //===========================================================
    // 函数名：on_closeButton_clicked
    // 功能：该类窗口界面上的关闭按钮的click()信号的槽函数，关闭窗口界面
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_closeButton_clicked();

    //===========================================================
    // 函数名：on_ChangePasswordBtn_clicked
    // 功能：该类窗口界面上的修改密码按钮的click()信号的槽函数，复用
    //      PasswordDialog类对应的窗口来修改密码
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_ChangePasswordBtn_clicked();

    //===========================================================
    // 函数名：receiveData
    // 功能：用于接收密码输入窗口中输入框内输入的字符串至成员变量m_password中
    // 输入参数：密码输入窗口中输入框内输入的字符串
    // 返回值：空
    //===========================================================
    void receiveData(QString date);

private:
    Ui::ShowStuDialog *ui;
    string m_stuNum;
    string m_password;

    //===========================================================
    // 函数名：updateTableWidget
    // 功能：根据传入学号更新窗口界面组件上显示的窗口信息
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void updateTableWidget();
};

#endif // SHOWSTUDIALOG_H
