#include "cosumedialog.h"
#include "ui_cosumedialog.h"

#include <QMessageBox>
#include <QRegExpValidator>
#include <QTextCodec>

//===========================================================
// ��������CosumeDialog
// ���ܣ�CosumeDialog��Ĺ��캯������ɳ�Ա����m_winNum�ĳ�ʼ����
//      �����ô��ڽ��������������������������
// ���������ʳ�ô��ں�-1�������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
CosumeDialog::CosumeDialog(int winNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CosumeDialog)
{
    ui->setupUi(this);
    this->m_winNum = winNum;

    // ���ÿ��������ֻ���������� ������볤����7λ eg.3123761
    ui->cardNumEdit->setMaxLength(7);
    QRegExp regx1("[0-9]+$");
    QValidator *validator1 = new QRegExpValidator(regx1,ui->cardNumEdit);
    ui->cardNumEdit->setValidator(validator1);

    // �������ѽ��ֻ������0-100�ڵ���λ������
    ui->addMoneyEdit->setMaxLength(5);
    QRegExp regx2("[0-9]{0,2}[.][0-9]{0,2}");
    QValidator *validator2 = new QRegExpValidator(regx2,ui->addMoneyEdit);
    ui->addMoneyEdit->setValidator(validator2);

}

//===========================================================
// ��������~CosumeDialog
// ���ܣ�CosumeDialog���������������CosumeDialog��Ķ����ͷ�ʱ
//      �ͷ�ui
// �����������
// ����ֵ����
//===========================================================
CosumeDialog::~CosumeDialog()
{
    delete ui;
}

//===========================================================
// ��������on_cancelButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void CosumeDialog::on_cancelButton_clicked()
{
    ui->cardNumEdit->clear();
    ui->addMoneyEdit->clear();
    this->close();
}

//===========================================================
// ��������on_okButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������������Ϣ
//      ����ϵͳ�е����Ѻ������ѧ�������Ѳ���
// �����������
// ����ֵ����
//===========================================================
void CosumeDialog::on_okButton_clicked()
{
    // ��������ַ��ڴ��ڽ��������������
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    // ����
    string cardNum = ui->cardNumEdit->text().toStdString();

    // ��������Ӧ����7λ���� ����������2-4������
    if(cardNum.size()<7){
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("���������ʽ��ȷ�Ŀ��ţ�"));
        return;
    }

    // ����������ѽ��Ϊ��
    if(ui->addMoneyEdit->text()==""){
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("�����������ѽ�"));
        return;
    }

    // ���ѽ��
    float conMoney = ui->addMoneyEdit->text().toFloat();

    // ��ȡʱ��
    auto DT = SetDateTime::getinstance();
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);

    // ����
    auto COP = CanteenOpration::getinstance();
    COP->consume(false,m_winNum,cardNum,conMoney,date,time);

    ui->cardNumEdit->clear();
    ui->addMoneyEdit->clear();
    this->close();
}
