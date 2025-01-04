#include "inputstunumdialog.h"
#include "ui_inputstunumdialog.h"

#include <QRegExpValidator>
#include <QMessageBox>
#include <QTextCodec>

//===========================================================
// ��������inPutStuNumDialog
// ���ܣ�inPutStuNumDialog��Ĺ��캯������ɳ�Ա����m_choice�ĳ�ʼ����
//      ������ѡ�����øø��ô��ڽ���Ĵ�������ͬʱ������������������
// ���������ѡ�����������ڲ�����ָ�루û�и�������ᴫ��nullptr��
// ����ֵ����
//===========================================================
inPutStuNumDialog::inPutStuNumDialog(int choice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inPutStuNumDialog)
{
    ui->setupUi(this);

    m_choice = choice;

    // ����ѡ�����ô��ڱ���
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    switch(choice){
    case 1: // 1ѡ�񲹿�
       this->setWindowTitle(codec->toUnicode("У԰������"));
        break;
    case 2: // 2ѡ���ʧ
        this->setWindowTitle(codec->toUnicode("У԰����ʧ"));
        break;
    case 3: // 3ѡ����
        this->setWindowTitle(codec->toUnicode("У԰�����"));
        break;
    case 4: // 4ѡ������
        this->setWindowTitle(codec->toUnicode("ѧ������"));
        break;
    default:
        break;
    }

    // ����ѧ�������ֻ���������� ������볤����ʮλ eg.2020121753
    ui->stuNumEdit->setMaxLength(10);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->stuNumEdit);
    ui->stuNumEdit->setValidator(validator);
}

//===========================================================
// ��������~inPutStuNumDialog
// ���ܣ�inPutStuNumDialog���������������CosumeDialog��Ķ�����
//      ��ʱ�ͷ�ui
// �����������
// ����ֵ����
//===========================================================
inPutStuNumDialog::~inPutStuNumDialog()
{
    delete ui;
}

//===========================================================
// ��������on_okButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȷ�ϰ�ť��click()�źŵĲۺ���������ѡ�����
//      ��������Ϣ����ϵͳ�ж�Ӧ�Ĺ��ܺ�����m_choice��1,������2,��ʧ��
//      3,��ң�4,������
// �����������
// ����ֵ����
//===========================================================
void inPutStuNumDialog::on_okButton_clicked()
{
    // ѧ��
    string stuNum = ui->stuNumEdit->text().toStdString();

    // ѧ������Ӧ����ʮλ����
    if(stuNum.size()<10){
        // ��������ַ��ڴ��ڽ��������������
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(this,codec->toUnicode("��ʾ"),
                             codec->toUnicode("���������ʽ��ȷ��ѧ�ţ�"));
        return;
    }

    // ��ȡʱ��
    auto DT = SetDateTime::getinstance();
    string date = DT->date().toString("yyyyMMdd").toStdString();
    string time = DT->time().toString("hhmmsszzz").toStdString().substr(0,8);

    // ��ɶ�Ӧ�Ĳ���
    auto CO = CardOpration::getinstance();
    switch(m_choice){
    case 1: // 1ѡ�񲹿�
        CO->makeUpCard(date,time,stuNum);
        break;
    case 2: // 2ѡ���ʧ
        CO->lossSolution_1(date,time,stuNum);
        break;
    case 3: // 3ѡ����
        CO->lossSolution_2(date,time,stuNum);
        break;
    case 4: // 4ѡ������
        CO->deleteStu(date,time,stuNum);
        break;
    default:
        break;
    }

    ui->stuNumEdit->clear();
    this->close();
}

//===========================================================
// ��������on_cancelButton_clicked
// ���ܣ����ര�ڽ����ϵ�ȡ����ť��click()�źŵĲۺ������رմ��ڽ���
// �����������
// ����ֵ����
//===========================================================
void inPutStuNumDialog::on_cancelButton_clicked()
{
    ui->stuNumEdit->clear();
    this->close();
}
