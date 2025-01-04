#ifndef COSUMEDIALOG_H
#define COSUMEDIALOG_H

#pragma once

#include <QDialog>
#include "canteenopration.h"
#include "setdatetime.h"

namespace Ui {
class CosumeDialog;
}

class CosumeDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // ��������CosumeDialog
    // ���ܣ�CosumeDialog��Ĺ��캯������ɳ�Ա����m_winNum�ĳ�ʼ����
    //      �����ô��ڽ��������������������������
    // ���������ʳ�ô��ں�-1�������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit CosumeDialog(int winNum, QWidget *parent = nullptr);

    //===========================================================
    // ��������~CosumeDialog
    // ���ܣ�CosumeDialog���������������CosumeDialog��Ķ����ͷ�ʱ
    //      �ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~CosumeDialog();

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
    // ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������������Ϣ
    //      ����ϵͳ�е����Ѻ������ѧ�������Ѳ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_okButton_clicked();

private:
    Ui::CosumeDialog *ui;
    int m_winNum;
};

#endif // COSUMEDIALOG_H
