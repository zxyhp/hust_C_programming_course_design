#include "windowdialog.h"
#include "ui_windowdialog.h"

//===========================================================
// ��������WindowDialog
// ���ܣ�WindowDialog��Ĺ��캯������ʼ����Ա����m_winNu��ͬʱ���
//      ʱ���������(dateTimeEdit)�ĳ�ʼ��������µ�����
// ������������ں�-1��ʱ�����ڱ����������ڲ�����ָ�루û�и�������
//          �ᴫ��nullptr��
// ����ֵ����
//===========================================================
WindowDialog::WindowDialog(int winNum,QDateTime dt,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowDialog)
{
    ui->setupUi(this);

    this->m_winNum = winNum;
    ui->winNumLabel->setNum(winNum);

    // ��ʼ��DateTimeEdit��ʱ��
    ui->dateTimeEdit->setDateTime(dt);
    // ����ʱ��仯����
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1);  // ÿ1ms�ͻᷢ���ź�timeout()

    // ��ʼ�����������ܴ������ܽ�����ʾ���
    this->updateLCDNum();
}

//===========================================================
// ��������~WindowDialog
// ���ܣ�WindowDialog���������������WindowDialog��Ķ����ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
WindowDialog::~WindowDialog()
{
    delete ui;
}

//===========================================================
// ��������on_pushButton_clicked
// ���ܣ����ര�ڽ����ϵķ��ذ�ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void WindowDialog::on_pushButton_clicked()
{
    this->close();
}

//===========================================================
// ��������on_consumeButton_clicked
// ���ܣ����ര�ڽ����ϵ����Ѱ�ť��click()�źŵĲۺ����������Ѵ���
//      ����
// �����������
// ����ֵ����
//===========================================================
void WindowDialog::on_consumeButton_clicked()
{
    CosumeDialog cosume(this->m_winNum-1);
    cosume.exec();
    this->updateLCDNum();
}

//===========================================================
// ��������on_csLogButton_clicked
// ���ܣ����ര�ڽ����ϵĲ鿴������־��ť��click()�źŵĲۺ�������
//      ������־���ڽ���
// �����������
// ����ֵ����
//===========================================================
void WindowDialog::on_csLogButton_clicked()
{
    CosumeLogDialog cosumeLog(this->m_winNum-1);
    cosumeLog.exec();
}

//===========================================================
// ��������timeUpdate
// ���ܣ����´��ڽ����е�dateTimeEdit�������ʾ����ʱ�䣬ÿ�θ��¼�1ms
// �����������
// ����ֵ����
//===========================================================
void WindowDialog::timeUpdate()
{
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
}

//===========================================================
// ��������updateLCDNum
// ���ܣ����´��ڽ�������ʾ�Ĵ��ڵ��������ܴ������ܽ���ʾ��
// �����������
// ����ֵ����
//===========================================================
void WindowDialog::updateLCDNum()
{
    auto COP = CanteenOpration::getinstance();
    pair<int,float> p = COP->getWinConsume(this->m_winNum-1);
    ui->csTimesLCD->display(QString::number(p.first));
    ui->csMoneyLCD->display(QString::number(p.second,'f',2));
}
