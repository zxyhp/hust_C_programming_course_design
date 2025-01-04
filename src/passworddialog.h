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
    // ��������PasswordDialog
    // ���ܣ�PasswordDialog��Ĺ��캯�������ô��ڽ����������������������
    // ��������������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit PasswordDialog(QWidget *parent = nullptr);

    //===========================================================
    // ��������~PasswordDialog
    // ���ܣ�PasswordDialog���������������PasswordDialog��Ķ����ͷ�ʱ
    //      �ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~PasswordDialog();

    //===========================================================
    // ��������changeTitle
    // ���ܣ��ı䴰�����ͱ����ǩ������
    // ����������´��������±����ǩ����
    // ����ֵ����
    //===========================================================
    void changeTitle(QString winTitle,QString labelStr);

signals:
    void sendData(QString);

private slots:

    //===========================================================
    // ��������on_cancelButton_clicked
    // ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_cancelButton_clicked();

    //===========================================================
    // ��������on_okButton_clicked
    // ���ܣ�������������ݽ��������봫�������ڵĳ�Ա����
    // �����������
    // ����ֵ����
    //===========================================================
    void on_okButton_clicked();

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
