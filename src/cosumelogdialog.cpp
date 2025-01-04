#include "cosumelogdialog.h"
#include "ui_cosumelogdialog.h"

//===========================================================
// ��������CosumeLogDialog
// ���ܣ�CosumeLogDialog��Ĺ��캯������ɳ�Ա����m_winNum�ĳ�ʼ����
//      ������Ӧ���ںŵ����Ѽ�¼��Ϣͬ�������ڽ���ı������
// ���������ʳ�ô��ں�-1�������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
CosumeLogDialog::CosumeLogDialog(int num, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CosumeLogDialog)
{
    ui->setupUi(this);

    this->m_winNum = num;
    // ���ô��ڽ���ı����ǩ
    ui->winNumLabel->setText(QString::number(m_winNum+1));
    // ���±�����
    this->updateTable();
}

//===========================================================
// ��������~CosumeLogDialog
// ���ܣ�CosumeLogDialog���������������CosumeLogDialog��Ķ�����
//      ��ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
CosumeLogDialog::~CosumeLogDialog()
{
    delete ui;
}


//===========================================================
// ��������updateTable
// ���ܣ�����Ӧ���ںŵ����Ѽ�¼������ʾ�����ڽ����tableWidget�����
// �����������
// ����ֵ����
//===========================================================
void CosumeLogDialog::updateTable()
{
    // ��ȡʳ�ö���
    auto COP = CanteenOpration::getinstance();
    winXfHead hp = COP->getXfRecords(m_winNum);
    int lineOfNow = COP->getWinLine(m_winNum);
    int num = hp.num;
    // ����TableWidget������
    ui->tableWidget->setRowCount(FILELINES);
    // ��������
    winXfStr *p = hp.now->previous;
    string str;
    while(num>0){
        lineOfNow = (lineOfNow-1+FILELINES)%FILELINES;
        // ���� eg.2021-10-05
        str = p->date.substr(0,4)+"-"+p->date.substr(4,2)+"-"+p->date.substr(6,2);
        ui->tableWidget->setItem(lineOfNow,0,new QTableWidgetItem(QString::fromStdString(str)));
        // ʱ�� eg.07:00:11.72
        str = p->time.substr(0,2)+":"+p->time.substr(2,2)+":"+p->time.substr(4,2)+"."+p->time.substr(6,2);
        ui->tableWidget->setItem(lineOfNow,1,new QTableWidgetItem(QString::fromStdString(str)));
        // ����
        ui->tableWidget->setItem(lineOfNow,2,new QTableWidgetItem(QString::fromStdString(p->cardNum)));
        // ���ѽ��
        ui->tableWidget->setItem(lineOfNow,3,new QTableWidgetItem(QString::number(p->money,'f',2)));
        num--;
        p=p->previous;
    }
}

//===========================================================
// ��������on_goBackButton_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CosumeLogDialog::on_goBackButton_clicked()
{
    this->close();
}


