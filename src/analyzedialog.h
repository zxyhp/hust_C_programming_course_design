#ifndef ANALYZEDIALOG_H
#define ANALYZEDIALOG_H

#include <QDialog>
#include "cardoprotion.h"

namespace Ui {
class AnalyzeDialog;
}

class AnalyzeDialog : public QDialog
{
    Q_OBJECT

public:
    //===========================================================
    // ��������AnalyzeDialog
    // ���ܣ�AnalyzeDialog��Ĺ��캯������ʼ����Ա����m_allReacords��
    //      m_sortedRecords��������ѧ����������������
    // ����������������ڵ����Ѽ�¼��Ϣ����ڼ�ֵ�Լ��ϣ��������Ѽ�¼�����
    //         ͷ�ڵ��ָ�룻�����ڲ�����ָ�루û�и�������ᴫ��nullptr��
    // ����ֵ����
    //===========================================================
    explicit AnalyzeDialog(map<int,pair<int,winXfStr*>> m, sortedXf* sortedRecords, QWidget *parent = nullptr);

    //===========================================================
    // ��������~AnalyzeDialog
    // ���ܣ�AnalyzeDialog���������������AnalyzeDialog��Ķ����ͷ�ʱ�ͷ�ui
    // �����������
    // ����ֵ����
    //===========================================================
    ~AnalyzeDialog();

private slots:
    //===========================================================
    // ��������on_backBtn_clicked
    // ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
    // �����������
    // ����ֵ����
    //===========================================================
    void on_backBtn_clicked();

    //===========================================================
    // ��������on_analyzeBtn_clicked
    // ���ܣ����ര�ڽ����ϵķ�����ť��click()�źŵĲۺ�����������������
    //      ѧ�Ŷ�Ӧѧ������һ����ʳ���������ѵ�ѧ���������ǵ���Ϣ�����
    //      Ա����m_info��
    // �����������
    // ����ֵ����
    //===========================================================
    void on_analyzeBtn_clicked();

private:
    Ui::AnalyzeDialog *ui;
    // ʳ�ô��ڵ����м�¼
    // <���ںţ�<���ݸ�����������һ����Ϣ��д��λ��>>
    map<int,pair<int,winXfStr*>> m_allReacords;
    // ��¼��ĳλͬѧ����һ���Ŷӵ�ͬѧ��Ϣ
    // <ѧ�ţ�<����������> >
    map<string,pair<int,string>> m_info;
    // ���д��ڼ�¼�����򼯺�����ͷ
    sortedXf* m_sortedRecords;

    //===========================================================
    // ��������updateTable
    // ���ܣ����³�Ա����m_info����Ϣ�����ڽ����tableWidget�����
    // �����������
    // ����ֵ����
    //===========================================================
    void updateTable();
};

#endif // ANALYZEDIALOG_H
