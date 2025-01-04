#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// ��������StatisticsDialog
// ���ܣ�StatisticsDialog��Ĺ��캯������ɳ�Ա����m_sortedRecords
//      �ĳ�ʼ��
// �������������������־��������ͷ�ڵ�ָ�룻�����ڲ�����ָ�루û
//          �и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
StatisticsDialog::StatisticsDialog(sortedXf *xf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog)
{
    ui->setupUi(this);
    this->m_sortedRecords = xf;

    // ����ѧ�������ֻ���������� ������볤����ʮλ eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator);
}

//===========================================================
// ��������~StatisticsDialog
// ���ܣ�StatisticsDialog���������������StatisticsDialog��Ķ���
//      �ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
StatisticsDialog::~StatisticsDialog()
{
    delete ui;
}

//===========================================================
// ��������on_backButton_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void StatisticsDialog::on_backButton_clicked()
{
    this->close();
}

//===========================================================
// ��������on_inquireButton_clicked
// ���ܣ����ര�ڽ����ϵĲ�ѯ��ť��click()�źŵĲۺ���������������Ϣ
//      ��������޶����ں�ѧ�ţ�����ѧ���ڸö�ʱ������Ѵ����ͽ�
//      ��ʾ�ڴ��ڽ���������
// �����������
// ����ֵ����
//===========================================================
void StatisticsDialog::on_inquireButton_clicked()
{
    // �����ѧ��
    string stuNum = ui->stuNumEdit->text().toStdString();

    if(stuNum.size()<10){
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("�������ʽ��ȷ��ѧ�ţ�"));
        return;
    }

    // �����ܴ���
    int allTimes = 0;
    // �����ܽ��
    float allConMoney = 0;

    // ��������ѧ�ò����� �������ѯ
    auto CO = CardOpration::getinstance();
    if(CO->students.find(stuNum) == CO->students.end()){
        ui->timesLCD->display(QString::number(allTimes));
        ui->moneyLCD->display(QString::number(allConMoney,'f',2));
        return;
    }

    // ��ʼ����
    string startDate = ui->startDateEdit->date().toString("yyyyMMdd").toStdString();
    // ��������
    string endDate = ui->endDateEdit->date().toString("yyyyMMdd").toStdString();

    // �����ʼ���ڴ��ڽ������������
    if(startDate>endDate) ;
    // �����ʼ���ڱ�������¼��������ڶ��������
    else if(startDate>this->m_sortedRecords->date.substr(0,8)) ;
    // �����¼Ϊ�գ���������ڱ�������¼����С���ڶ�С�����
    else if(this->m_sortedRecords->next==nullptr || endDate<this->m_sortedRecords->next->date ) ;
    //��������
    else{
        // �ȶ�λ��ָ�����ڷ�Χ��
        sortedXf* p = this->m_sortedRecords->next;
        // �ҵ�p->date>=startDate��p
        while(p->date < startDate) p = p->next;
        // ���ƶ���Χ��������Ѱ���ƶ�ѧ�ŵļ�¼
        while(p!=nullptr && p->date <= endDate){
            if(p->stuNum == stuNum){
                allTimes++;
                allConMoney += p->money;
            }
            p = p->next;
        }
    }

    // �ڴ��ڽ����������ʾ�������
    ui->timesLCD->display(QString::number(allTimes));
    ui->moneyLCD->display(QString::number(allConMoney,'f',2));
}
