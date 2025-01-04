#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#pragma once
#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    //===========================================================
    // 函数名：PasswordDialog
    // 功能：PasswordDialog类的构造函数，设置窗口界面的输入框的输入内容限制
    // 输入参数：父窗口部件的指针（没有父窗口则会传入nullptr）
    // 返回值：空
    //===========================================================
    explicit PasswordDialog(QWidget *parent = nullptr);

    //===========================================================
    // 函数名：~PasswordDialog
    // 功能：PasswordDialog类的析构函数，在PasswordDialog类的对象释放时
    //      释放ui
    // 输入参数：空
    // 返回值：空
    //===========================================================
    ~PasswordDialog();

    //===========================================================
    // 函数名：changeTitle
    // 功能：改变窗口名和标题标签的内容
    // 输入参数：新窗口名；新标题标签内容
    // 返回值：空
    //===========================================================
    void changeTitle(QString winTitle,QString labelStr);

signals:
    void sendData(QString);

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
    // 功能：根据输入框内容将输入密码传给父窗口的成员变量
    // 输入参数：空
    // 返回值：空
    //===========================================================
    void on_okButton_clicked();

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
