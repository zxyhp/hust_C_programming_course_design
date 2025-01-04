#include "createstu.h"
#include "ui_createstu.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// ��������CreateStu
// ���ܣ�CreateStu��Ĺ��캯�������ô��ڽ��������������������������
// ��������������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
CreateStu::CreateStu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateStu)
{
    ui->setupUi(this);

    // ����ѧ�������ֻ���������� ������볤����ʮλ eg.2020121753
    ui->StuNumEdit->setMaxLength(10);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->StuNumEdit);
    ui->StuNumEdit->setValidator(validator1);

    // �������������ֻ�����뺺�� ������볤�����ĸ�����
    ui->nameEdit->setMaxLength(4);
    QRegExp regx2("[\u4e00-\u9fa5]+$");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->nameEdit);
    ui->nameEdit->setValidator(validator2);
}

//===========================================================
// ��������~CreateStu
// ���ܣ�CreateStu���������������CreateStu��Ķ����ͷ�ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
CreateStu::~CreateStu()
{
    delete ui;
}

//===========================================================
// ��������on_okButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������������Ϣ
//      ����ϵͳ�еĿ����������ѧ���Ŀ���
// �����������
// ����ֵ����
//===========================================================
void CreateStu::on_okButton_clicked()
{
    // ��������ַ��ڴ��ڽ��������������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ѧ��
    string stuNum = ui->StuNumEdit->text().toStdString();
    // ����
    string name = codec->fromUnicode(ui->nameEdit->text()).toStdString();

    // ѧ������Ӧ����ʮλ���� ����������2-4������
    if(stuNum.size()<10 || name.size()<4){
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("���������ʽ��ȷ��ѧ�ź�������"));
        return;
    }

    // ������ȷ�������Ӧ����
    auto CO = CardOpration::getinstance();
    auto DT = SetDateTime::getinstance();
    // ��ȡʱ��
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);
    // ����
    CO->createStu(date,time,stuNum,name);

    ui->StuNumEdit->clear();
    ui->nameEdit->clear();
    this->close();
}

//===========================================================
// ��������on_cancleButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CreateStu::on_cancleButton_clicked()
{
    ui->StuNumEdit->clear();
    ui->nameEdit->clear();
    this->close();
}
