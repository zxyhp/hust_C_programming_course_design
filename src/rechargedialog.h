#ifndef RECHARGEDIALOG_H
#define RECHARGEDIALOG_H

#pragma once
#include <QDialog>
#include "cardoprotion.h"
#include "setdatetime.h"

namespace Ui {
class RechargeDialog;
}

class RechargeDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // ��������RechargeDialog
    // ���ܣ�RechargeDialog��Ĺ��캯�������ô��ڽ����������������
    //      ����������
    // ��������������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit RechargeDialog(QWidget *parent = nullptr);

    //===========================================================
    // ��������~RechargeDialog
    // ���ܣ�RechargeDialog���������������RechargeDialog��Ķ����ͷ�ʱ
    //      �ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~RechargeDialog();

private slots:

    //===========================================================
    // ��������on_okButton_clicked
    // ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������������Ϣ
    //      ����ϵͳ�еĳ�ֵ�������ѧ�����˻���ֵ
    // �����������
    // ����ֵ����
    //===========================================================
    void on_okButton_clicked();

    //===========================================================
    // ��������on_cancelButton_clicked
    // ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_cancleButton_clicked();

private:
    Ui::RechargeDialog *ui;
};

#endif // RECHARGEDIALOG_H
