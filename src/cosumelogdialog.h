#ifndef COSUMELOGDIALOG_H
#define COSUMELOGDIALOG_H

#pragma once

#include <QDialog>
#include "canteenopration.h"

namespace Ui {
class CosumeLogDialog;
}

class CosumeLogDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // ��������CosumeLogDialog
    // ���ܣ�CosumeLogDialog��Ĺ��캯������ɳ�Ա����m_winNum�ĳ�ʼ����
    //      ������Ӧ���ںŵ����Ѽ�¼��Ϣͬ�������ڽ���ı������
    // ���������ʳ�ô��ں�-1�������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit CosumeLogDialog(int num, QWidget *parent = nullptr);

    //===========================================================
    // ��������~CosumeLogDialog
    // ���ܣ�CosumeLogDialog���������������CosumeLogDialog��Ķ�����
    //      ��ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~CosumeLogDialog();

private slots:

    //===========================================================
    // ��������on_goBackButton_clicked
    // ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_goBackButton_clicked();

private:
    Ui::CosumeLogDialog *ui;
    int m_winNum;

    //===========================================================
    // ��������updateTable
    // ���ܣ�����Ӧ���ںŵ����Ѽ�¼������ʾ�����ڽ����tableWidget�����
    // �����������
    // ����ֵ����
    //===========================================================
    void updateTable();
};

#endif // COSUMELOGDIALOG_H
