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
    // ��������ShowStuDialog
    // ���ܣ�ShowStuDialog��Ĺ��캯������ʼ����Ա����m_stuNum�����ݴ�
    //      ��ѧ��ѧ�Ÿ��´��ڽ�����Ϣ
    // ���������ѧ��ѧ�ţ������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit ShowStuDialog(string stuNum, QWidget *parent = nullptr);

    //===========================================================
    // ��������~ShowStuDialog
    // ���ܣ�ShowStuDialog���������������ShowStuDialog��Ķ����ͷ�
    //      ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~ShowStuDialog();

private slots:

    //===========================================================
    // ��������on_closeButton_clicked
    // ���ܣ����ര�ڽ����ϵĹرհ�ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_closeButton_clicked();

    //===========================================================
    // ��������on_ChangePasswordBtn_clicked
    // ���ܣ����ര�ڽ����ϵ��޸����밴ť��click()�źŵĲۺ���������
    //      PasswordDialog���Ӧ�Ĵ������޸�����
    // �����������
    // ����ֵ����
    //===========================================================
    void on_ChangePasswordBtn_clicked();

    //===========================================================
    // ��������receiveData
    // ���ܣ����ڽ����������봰�����������������ַ�������Ա����m_password��
    // ����������������봰�����������������ַ���
    // ����ֵ����
    //===========================================================
    void receiveData(QString date);

private:
    Ui::ShowStuDialog *ui;
    string m_stuNum;
    string m_password;

    //===========================================================
    // ��������updateTableWidget
    // ���ܣ����ݴ���ѧ�Ÿ��´��ڽ����������ʾ�Ĵ�����Ϣ
    // �����������
    // ����ֵ����
    //===========================================================
    void updateTableWidget();
};

#endif // SHOWSTUDIALOG_H
