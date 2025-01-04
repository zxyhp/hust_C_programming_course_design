#include "campuscard.h"
#include "ui_campuscard.h"

#include <QMessageBox>
#include <QTextCodec>
#include <QDir>

//===========================================================
// ��������CampusCard
// ���ܣ�CampusCard��Ĺ��캯������CampusCard��Ķ�������ʱ����
//      �˴���Ҫ��ɼ���ȫ��ָ������ĳ�ʼ��������ʱ��(QDateTime*)��
//      ѧ������(CardOpration*)��ʳ��Ӧ�ò���(CanteenOpration*)��
//      �Լ�����ʱ���������(dateTimeEdit)�ĸ��¹���
// ��������������ڲ�����ָ�루û�и���������nullptr��
// ����ֵ����
//===========================================================
CampusCard::CampusCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CampusCard)
{
    ui->setupUi(this);

    // ��ʼ��ʱ��
    auto DT = SetDateTime::getinstance();
    // ��ʼ�� ����ʱ����� ��ʱ��
    ui->dateTimeEdit->setDateTime(*DT);

    // ����ʱ��仯����
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    // ÿ1ms�ͻᷢ���ź�timeout() ����ui->dateTimeEdit����ʾʱ��
    timer->start(1);

    // ��ʼ��һ��ѧ����������Ķ���ָ��
    this->CO = CardOpration::getinstance();

    // ��ʼ��һ��ʳ��Ӧ�ò�������Ķ���ָ��
    this->COP = CanteenOpration::getinstance();
}

//===========================================================
// ��������~CampusCard
// ���ܣ�CampusCard���������������CampusCard��Ķ����ͷ�ʱ���ã�
//      �˴���Ҫ���ͷ�ui��д�������־
// �����������
// ����ֵ����
//===========================================================
CampusCard::~CampusCard()
{
    delete ui;
    // �ڳ���رյ�ʱ�򽫴����ڴ�Ĳ�����־���д���ļ���
    this->writeOpLog();
}

//===========================================================
// ��������writeOpLog
// ���ܣ���������־��������ڵ�����ȫ��д��ͬĿ¼�µ�oplog.txt�ļ���
// �����������
// ����ֵ����
//===========================================================
void CampusCard::writeOpLog()
{
    // �������в�����־�����ַ���
    string oplog = "";

    // ��ȡ ���������־��������ͷβ�ڵ�ָ�� �� ȫ��ָ�����(opLogHead*����)
    auto head = OprationLog::getinstance();

    // ��ȡ������־��������ͷ�ڵ�ָ��
    opLogStr *p = head->first;

    // ����������־������� �����е����ݶ��Ȼ�ȡ��oplog��
    while(p->next!=nullptr){
        p=p->next;
        oplog += p->str;
    }

    // ��oplogд��ͬĿ¼�µ�oplog.txt��
    string filePath = QDir::currentPath().toStdString() + "/oplog.txt";
    ofstream fp(filePath);
    fp<<oplog;
    fp.close();
}

//===========================================================
// ��������updateData
// ���ܣ������������ݵ�����LeadData�еĺ����������ļ��е�������������
// �����������
// ����ֵ����
//===========================================================
void CampusCard::updateData()
{
    clock_t startTime = clock();
    string tipStr = "�������ݹ�����ʱ�䣺";

    // ����һ���������ݵ������ָ��
    LeadData *LD = new LeadData;

    // ��ͬĿ¼�µ�students.txt�л�ȡ����ѧ����Ϣ������俪��
    // ͬʱ�ڿ���̨���initStu�������������ʱ��(s)
    LD->runTime(&LeadData::initStu);

    // ��ͬĿ¼�µ�cz.tzt��xf.txt�л�ȡѧ������������������Ϣ
    // ͬʱ�ڿ���̨���getOpration�������������ʱ��(s)
    LD->runTime(&LeadData::getOpration);

    // �� ��ȡ��ѧ������������������Ϣ ������ʱ�� ������������
    LD->runTime(&LeadData::sortOpration);

    // ��ϵͳ�����ѧ���Ĳ���������
    // ͬʱ��ȡ���һ�����������ں�ʱ�䣨���в������������ʱ�䣩
    pair<string,string> p = LD->carryOutOp();

    // ��������ʱ������ ���´������dateTimeEdit������ʾ������ʱ��
    ui->dateTimeEdit->setDate(QDate::fromString(QString::fromStdString(p.first),"yyyyMMdd"));
    ui->dateTimeEdit->setTime(QTime::fromString(QString::fromStdString(p.second+"0"),"hhmmsszzz"));

    // ����������ݵ�����ָ��
    delete LD;

    clock_t endTime = clock();

    // ����ʱ���ַ���
    stringstream ss;
    string timeStr;
    ss<<fixed<<setprecision(3)<<double(endTime - startTime)/CLOCKS_PER_SEC;
    ss>>timeStr;

    tipStr += timeStr + "s";

    // ������ʾ����ʱ��
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QMessageBox::information(this,codec->toUnicode("��ʾ"),
                         codec->toUnicode(tipStr.c_str()));
}

//===========================================================
// ��������on_quit_clicked
// ���ܣ����ര�ڽ����ϵ��˳���ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CampusCard::on_quit_clicked()
{
    this->close();
}

//===========================================================
// ��������timeUpdate
// ���ܣ����´��ڽ����е�dateTimeEdit�������ʾ����ʱ�䣬ÿ�θ��¼�
//      1ms��ͬʱҲ����ȫ��ʱ������ָ�������ָ������ʱ��
// �����������
// ����ֵ����
//===========================================================
void CampusCard::timeUpdate()
{
    auto DT = SetDateTime::getinstance();
    QDateTime time = ui->dateTimeEdit->dateTime().addMSecs(1);
    ui->dateTimeEdit->setDateTime(time);
    DT->setDate(time.date());
    DT->setTime(time.time());
}

//===========================================================
// ��������on_CardControl_clicked
// ���ܣ����ര�ڽ����ϵ�ѧ������ť��click()�źŵĲۺ�������ѧ��
//      ����Ĵ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CampusCard::on_CardControl_clicked()
{
    CardControl *cardcontrol = new CardControl(ui->dateTimeEdit->dateTime());
    cardcontrol->exec();
}

//===========================================================
// ��������on_CanteenApp_clicked
// ���ܣ����ര�ڽ����ϵ�ʳ��Ӧ�ð�ť��click()�źŵĲۺ�������ʳ��
//      Ӧ�õĴ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CampusCard::on_CanteenApp_clicked()
{
    CanteenAppDialog *canteenApp = new CanteenAppDialog(ui->dateTimeEdit->dateTime());
    canteenApp->exec();
}

//===========================================================
// ��������on_Summary_clicked
// ���ܣ����ര�ڽ����ϵĻ��ܷ�����ť��click()�źŵĲۺ������򿪻���
//      �����Ĵ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CampusCard::on_Summary_clicked()
{
    SummarizeDialog *summarize = new SummarizeDialog(this->COP);
    summarize->exec();
}

//===========================================================
// ��������on_inputAllBtn_clicked
// ���ܣ����ര�ڽ����ϵĵ����������ݰ�ť��click()�źŵĲۺ�����ѡ��
//      �Ƿ�����������
// �����������
// ����ֵ����
//===========================================================
void CampusCard::on_inputAllBtn_clicked()
{
    // ��ֹ�����ڴ��ڽ���������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ����ѡ���Ƿ���е�����Ĵ���
    QMessageBox msgBox;
    msgBox.setText(codec->toUnicode("�������ݽ��ɵ���һ�Σ��ҵ���󲻿ɳ�����"));
    msgBox.setInformativeText(codec->toUnicode("�Ƿ��������?"));
    msgBox.addButton(codec->toUnicode("��"),QMessageBox::AcceptRole);
    msgBox.addButton(codec->toUnicode("��"),QMessageBox::RejectRole);
    int ret = msgBox.exec();        // ѡ����Ϣ����ret����

    switch (ret) {
    // ���ѡ����
    case QMessageBox::AcceptRole:
        // �������ݰ��������ٵ��
        ui->inputAllBtn->setEnabled(false);
        // ���õ����������ݵĺ���
        this->updateData();
        break;

    // ���ѡ���
    case QMessageBox::RejectRole:
        break;

    default:
        // ���ᵽ������
        break;
    }

}
