#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#pragma once
#include <QDialog>
#include "cardoprotion.h"

namespace Ui {
class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:

    //===========================================================
    // ��������StatisticsDialog
    // ���ܣ�StatisticsDialog��Ĺ��캯������ɳ�Ա����m_sortedRecords
    //      �ĳ�ʼ��
    // �������������������־��������ͷ�ڵ�ָ�룻�����ڲ�����ָ�루û
    //          �и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit StatisticsDialog(sortedXf *xf, QWidget *parent = nullptr);

    //===========================================================
    // ��������~StatisticsDialog
    // ���ܣ�StatisticsDialog���������������StatisticsDialog��Ķ���
    //      �ͷ�ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~StatisticsDialog();

private slots:

    //===========================================================
    // ��������on_backButton_clicked
    // ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_backButton_clicked();

    //===========================================================
    // ��������on_inquireButton_clicked
    // ���ܣ����ര�ڽ����ϵĲ�ѯ��ť��click()�źŵĲۺ���������������Ϣ
    //      ��������޶����ں�ѧ�ţ�����ѧ���ڸö�ʱ������Ѵ����ͽ�
    //      ��ʾ�ڴ��ڽ���������
    // �����������
    // ����ֵ����
    //===========================================================
    void on_inquireButton_clicked();

private:
    Ui::StatisticsDialog *ui;
    sortedXf *m_sortedRecords;
};

#endif // STATISTICSDIALOG_H
