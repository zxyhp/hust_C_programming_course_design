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
    // ��������CreateStu
    // ���ܣ�CreateStu��Ĺ��캯�������ô��ڽ��������������������������
    // ��������������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit CreateStu(QWidget *parent = nullptr);

    //===========================================================
    // ��������~CreateStu
    // ���ܣ�CreateStu���������������CreateStu��Ķ����ͷ�ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~CreateStu();

private slots:

    //===========================================================
    // ��������on_okButton_clicked
    // ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������������Ϣ
    //      ����ϵͳ�еĿ����������ѧ���Ŀ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_okButton_clicked();

    //===========================================================
    // ��������on_cancleButton_clicked
    // ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_cancleButton_clicked();

private:
    Ui::CreateStu *ui;
};

#endif // CREATESTU_H
