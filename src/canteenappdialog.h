#ifndef CANTEENAPPDIALOG_H
#define CANTEENAPPDIALOG_H

#pragma once
#include <QDialog>
#include "windowdialog.h"
#include "setdatetime.h"

namespace Ui {
class CanteenAppDialog;
}

class CanteenAppDialog : public QDialog
{
    Q_OBJECT

public:
    //===========================================================
    // ��������CanteenAppDialog
    // ���ܣ�CanteenAppDialog��Ĺ��캯�������ʱ���������(dateTimeEdit)
    //      �ĳ�ʼ����������µ�����
    // ���������ʱ�����ڱ����������ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit CanteenAppDialog(QDateTime dt, QWidget *parent = nullptr);

    //===========================================================
    // ��������~CanteenAppDialog
    // ���ܣ�CanteenAppDialog���������������CanteenAppDialog��Ķ���
    //      �ͷ�ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~CanteenAppDialog();

private slots:

    //===========================================================
    // ��������on_goBackButton_clicked
    // ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_goBackButton_clicked();

    //===========================================================
    // ��������on_winTable_cellDoubleClicked
    // ���ܣ����ര�ڽ����ϵ�winTable����ĵ�Ԫ��˫���źŵĲۺ�����������
    //      �򿪵�Ԫ���Ӧ��ʳ�����Ѵ��ڽ���
    // �����������ѡ�е�Ԫ������±꣬��ѡ�е�Ԫ������±�
    // ����ֵ����
    //===========================================================
    void on_winTable_cellDoubleClicked(int row, int column);

    //===========================================================
    // ��������timeUpdate
    // ���ܣ����´��ڽ����е�dateTimeEdit�������ʾ����ʱ�䣬ÿ�θ��¼�1ms
    // �����������
    // ����ֵ����
    //===========================================================
    void timeUpdate();

private:
    Ui::CanteenAppDialog *ui;
};

#endif // CANTEENAPPDIALOG_H
