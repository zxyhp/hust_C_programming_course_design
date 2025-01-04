#include "rechargedialog.h"
#include "ui_rechargedialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// ��������RechargeDialog
// ���ܣ�RechargeDialog��Ĺ��캯�������ô��ڽ����������������
//      ����������
// ��������������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
RechargeDialog::RechargeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RechargeDialog)
{
    ui->setupUi(this);

    // ����ѧ�������ֻ���������� ������볤����10λ eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator1);

    // ���ó�ֵ�������� ֻ������1-999������
    ui->moneyEdit->setMaxLength(3);
    QRegExp regx2("[1-9][0-9]+$");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->moneyEdit);
    ui->moneyEdit->setValidator(validator2);
}

//===========================================================
// ��������~RechargeDialog
// ���ܣ�RechargeDialog���������������RechargeDialog��Ķ����ͷ�ʱ
//      �ͷ�ui
// �����������
// ����ֵ����
//===========================================================
RechargeDialog::~RechargeDialog()
{
    delete ui;
}

//===========================================================
// ��������on_okButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������������Ϣ
//      ����ϵͳ�еĳ�ֵ�������ѧ�����˻���ֵ
// �����������
// ����ֵ����
//===========================================================
void RechargeDialog::on_okButton_clicked()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ѧ��
    string stuNum = ui->stuNumEdit->text().toStdString();
    // ѧ������Ӧ����ʮλ����
    if(stuNum.size()<10){
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("���������ʽ��ȷ��ѧ�ţ�"));
        return;
    }

    // ��ֵ����Ϊ��
    if(ui->moneyEdit->text()==""){
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("���������ֵ��"));
    }

    // ��ֵ���
    float money = ui->moneyEdit->text().toFloat();

    auto CO = CardOpration::getinstance();
    auto DT = SetDateTime::getinstance();
    // ��ȡʱ��
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);
    // ��ֵ
    CO->recharge(date,time,stuNum,money);

    ui->stuNumEdit->clear();
    ui->moneyEdit->clear();
    this->close();
}

//===========================================================
// ��������on_cancelButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void RechargeDialog::on_cancleButton_clicked()
{
    ui->stuNumEdit->clear();
    ui->moneyEdit->clear();
    this->close();
}
