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
    // ��������inPutStuNumDialog
    // ���ܣ�inPutStuNumDialog��Ĺ��캯������ɳ�Ա����m_choice�ĳ�ʼ����
    //      ������ѡ�����øø��ô��ڽ���Ĵ�������ͬʱ������������������
    // ���������ѡ�����������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit inPutStuNumDialog(int choice, QWidget *parent = nullptr);

    //===========================================================
    // ��������~inPutStuNumDialog
    // ���ܣ�inPutStuNumDialog���������������CosumeDialog��Ķ�����
    //      ��ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~inPutStuNumDialog();

private slots:

    //===========================================================
    // ��������on_okButton_clicked
    // ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������ѡ�����
    //      ��������Ϣ����ϵͳ�ж�Ӧ�Ĺ��ܺ�����m_choice��1,������2,��ʧ��
    //      3,��ң�4,������
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
    void on_cancelButton_clicked();

private:
    Ui::inPutStuNumDialog *ui;
    int m_choice;
};

#endif // INPUTSTUNUMDIALOG_H
